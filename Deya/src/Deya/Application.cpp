#include "dypch.h"

#include <GLFW/glfw3.h>

#include "Deya/Application.h"
#include "Deya/Window.h"
#include "Deya/Input.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/Renderer.h"

namespace Deya
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;    

	Application::Application()
	{
		DY_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());
	
		/**
		 * !Legume!
		 */
		
		// legume verts
		float vertices[6 * 7] = // 6 verts * (3 dimensions + 4 colour channels)
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

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
	
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		/**
		 * !MANS!
		 */

		float mansVerts[17 * 7] =	// 17 verts * (3 dimensions + 4 colour channels)
		{
			// POS						COLOUR
			// mans head
			 0.0f,  0.8f,  0.0f,        0.92f, 0.82f, 0.76f, 0.0f,      // top vert    			#ebd2c1     0
			 0.0f,  0.4f,  0.0f,        0.92f, 0.82f, 0.76f, 0.0f,      // bottom vert     		#ebd2c1     1
			-0.2f,  0.6f,  0.0f,        0.92f, 0.82f, 0.76f, 0.0f,      // left-mid vert     	#ebd2c1     2
			 0.2f,  0.6f,  0.0f,        0.92f, 0.82f, 0.76f, 0.0f,      // right-mid vert   	#ebd2c1     3
			// mans body
			 0.0f,  0.4f,  0.0f,        0.22f, 0.41f, 0.76f, 0.0f,		// top vert				#3868c2		4
			 0.0f, -0.4f,  0.0f,		0.22f, 0.41f, 0.76f, 0.0f,		// bottom vert			#3868c2		5
			-0.4f,  0.0f,  0.0f,		0.22f, 0.41f, 0.76f, 0.0f,		// left-mid vert		#3868c2		6
			 0.4f,  0.0f,  0.0f,		0.22f, 0.41f, 0.76f, 0.0f,		// right-mid vert		#3868c2		7
			// mans left leg
			-0.4f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,      // top vert   			#ebd2c1     8
			 0.0f, -0.4f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,      // mid vert   			#ebd2c1     9
			-0.4f, -0.8f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,      // bottom vert   		#ebd2c1     10
			// mans right leg
			 0.4f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,      // top vert   			#ebd2c1     11
			//* (9)														// mid vert				#ebd2c1
			 0.4f, -0.8f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,      // bottom vert   		#ebd2c1     12
			// mans left arm
			-0.6f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,		// bottom-right vert   	#ebd2c1     13
			//* (8)														// bottom-left vert		#ebd2c1
			-0.6f,  0.2f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,		// top vert   			#ebd2c1     14
			// mans right arm
			//* (11)													// bottom-right vert	#ebd2c1
			 0.6f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,		// bottom-left vert   	#ebd2c1     15
			 0.6f,  0.2f,  0.0f,		0.92f, 0.82f, 0.76f, 0.0f,		// top vert   			#ebd2c1     16
		};

		uint32_t mansIndices[24] = // the order to render our verts
		{
			// mans head
			2, 1, 0,		// left tri
			1, 3, 0,		// right tri
			// mans body
			6, 5, 4,		// left tri
			5, 7, 4,		// right tri
			// mans left leg
			8, 10, 9,
			// mans right leg
			11, 12, 9,
			// mans left arm
			13, 8, 14,
			// mans right arm
			11, 15, 16
		};

		m_MansVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> mansVB;
		mansVB.reset(VertexBuffer::Create(mansVerts, sizeof(mansVerts)));
		mansVB->SetLayout(layout);
		m_MansVA->AddVertexBuffer(mansVB);

		std::shared_ptr<IndexBuffer> mansIB;
		mansIB.reset(IndexBuffer::Create(mansIndices, sizeof(mansIndices) / sizeof(uint32_t)));
		m_MansVA->SetIndexBuffer(mansIB);

		/**
		 * !Shaders!
		 */

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

		std::string vertexSrcMoved = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position + 0.5, 1.0);
			}
		)";

		m_ShaderMoved.reset(new Shader(vertexSrcMoved, fragmentSrc));
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
			RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			Renderer::Submit(m_MansVA);

			m_ShaderMoved->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

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