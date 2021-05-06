#include "dypch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Deya/Application.h"
#include "Deya/Window.h"
#include "Deya/Input.h"
#include "Deya/Renderer/Shader.h"

namespace Deya
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Deya::ShaderDataType::Float:		return GL_FLOAT;
            case Deya::ShaderDataType::Float2:		return GL_FLOAT;
            case Deya::ShaderDataType::Float3:		return GL_FLOAT;
            case Deya::ShaderDataType::Float4:		return GL_FLOAT;
            case Deya::ShaderDataType::Int:			return GL_INT;
            case Deya::ShaderDataType::Int2:		return GL_INT;
            case Deya::ShaderDataType::Int3:		return GL_INT;
            case Deya::ShaderDataType::Int4:		return GL_INT;
            case Deya::ShaderDataType::Mat3:		return GL_FLOAT;
            case Deya::ShaderDataType::Mat4:		return GL_FLOAT;
            case Deya::ShaderDataType::Bool:		return GL_BOOL;
        }

		DY_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
    }

    Application::Application()
    {
        DY_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);
    
        // legume verts
        float vertices[6 * 7] = // 4 verts * (3 dimensions + 4 colour channels)
        {
            // middle of legume
			// POS						COLOUR
            -0.125f,  0.3f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // top left
            -0.125f, -0.3f, 0.0f, 		0.25f, 0.13f, 0.03f, 0.0f, // bottom left
             0.125f,  0.3f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // top right
             0.125f, -0.3f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // bottom right

            // top and bottom verts
             0.0f,  0.45f, 0.0f,		0.35f, 0.23f, 0.05f, 0.0f, // top
             0.0f, -0.45f, 0.0f,		0.15f, 0.03f, 0.01f, 0.0f  // bottom
        };

        uint32_t indices[12] = // the order to render our verts
        {
            0, 1, 2,  // square tri left
            1, 3, 2,  // square tri right
            0, 2, 4,  // top tri
            1, 5, 3   // bottom tri
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

        uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer( 								//*Tell OpenGL what our data means
				index,											// Index of attribute
				element.GetComponentCount(),					// How many components there are in the attribute (vec2 = 2, vec3 = 3, ...)
				ShaderDataTypeToOpenGLBaseType(element.Type), 	// The type of attribute this is (float, int, ...)
				element.Normalized ? GL_TRUE : GL_FALSE, 		// Is the data normalized?
				layout.GetStride(),								// The stride between the attributes
				(const void*) element.Offset); 					// Offset between the start of the data, and the attribute			
            index++;
        }

        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
			out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
				v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
			in vec4 v_Color;

            void main()
            {
                //color = vec4(0.31, 0.16, 0.15, 1.0); // BROWN (#4f2a26)
                //color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1); // grey (#191919)
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        DY_CORE_INFO("Closing...");
        m_Running = false;
        return true;
    }
}