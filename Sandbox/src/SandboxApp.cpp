#include <Deya.h>
#include <Deya/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Deya::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), 
            m_CameraController(1280.0f / 720.0f, true),
            m_BackgroundColour({0.1f, 0.1f, 0.1f, 0.0f}),
            m_LegumeColour1({0.8f, 0.2f, 0.3f, 1.0f}),
            m_LegumeColour2({0.3f, 0.2f, 0.8f, 1.0f})
    {
        m_LegumeVA = Deya::VertexArray::Create();
	
		/**
		 * !Legume!
		 */
		
		// legume verts
		float vertices[6 * 3] = // 6 verts * (3 dimensions)
		{
			// middle of legume
			// POS						
			-0.125f,  0.2f, 0.0f		// top left
			-0.125f, -0.2f, 0.0f, 		// bottom left
			 0.125f,  0.2f, 0.0f,		// top right
			 0.125f, -0.2f, 0.0f,		// bottom right

			// top and bottom verts
			 0.0f,  0.35f, 0.0f,		// top
			 0.0f, -0.35f, 0.0f			// bottom
		};

		uint32_t indices[12] = // the order to render our verts
		{
			0, 1, 2,  // square tri left
			1, 3, 2,  // square tri right
			0, 2, 4,  // top tri
			1, 5, 3   // bottom tri
		};

		m_VertexBuffer.reset(Deya::VertexBuffer::Create(vertices, sizeof(vertices)));

		Deya::BufferLayout layout =
		{
			{ Deya::ShaderDataType::Float3, "a_Position" },
			{ Deya::ShaderDataType::Float4, "a_Color" }
		};

		Deya::BufferLayout layoutPlain =
		{
			{ Deya::ShaderDataType::Float3, "a_Position"}
		};

		Deya::BufferLayout layoutTexture =
		{
			{ Deya::ShaderDataType::Float3, "a_Position" },
			{ Deya::ShaderDataType::Float2, "a_TexCoord" },
		};
	
		m_VertexBuffer->SetLayout(layoutPlain);
		m_LegumeVA->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer.reset(Deya::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_LegumeVA->SetIndexBuffer(m_IndexBuffer);

		/**
		 * !MANS!
		 */

		float mansVerts[17 * 7] =	// 17 verts * (3 dimensions + 4 colour channels)
		{
			// POS						COLOUR
			// mans head
			 0.0f,  0.8f,  0.0f,        0.92f, 0.82f, 0.76f, 1.0f,      // top vert    			#ebd2c1     0
			 0.0f,  0.4f,  0.0f,        0.92f, 0.82f, 0.76f, 1.0f,      // bottom vert     		#ebd2c1     1
			-0.2f,  0.6f,  0.0f,        0.92f, 0.82f, 0.76f, 1.0f,      // left-mid vert     	#ebd2c1     2
			 0.2f,  0.6f,  0.0f,        0.92f, 0.82f, 0.76f, 1.0f,      // right-mid vert   	#ebd2c1     3
			// mans body
			 0.0f,  0.4f,  0.0f,        0.22f, 0.41f, 0.76f, 1.0f,		// top vert				#3868c2		4
			 0.0f, -0.4f,  0.0f,		0.22f, 0.41f, 0.76f, 1.0f,		// bottom vert			#3868c2		5
			-0.4f,  0.0f,  0.0f,		0.22f, 0.41f, 0.76f, 1.0f,		// left-mid vert		#3868c2		6
			 0.4f,  0.0f,  0.0f,		0.22f, 0.41f, 0.76f, 1.0f,		// right-mid vert		#3868c2		7
			// mans left leg
			-0.4f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,      // top vert   			#ebd2c1     8
			 0.0f, -0.4f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,      // mid vert   			#ebd2c1     9
			-0.4f, -0.8f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,      // bottom vert   		#ebd2c1     10
			// mans right leg
			 0.4f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,      // top vert   			#ebd2c1     11
			//* (9)														// mid vert				#ebd2c1
			 0.4f, -0.8f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,      // bottom vert   		#ebd2c1     12
			// mans left arm
			-0.6f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,		// bottom-right vert   	#ebd2c1     13
			//* (8)														// bottom-left vert		#ebd2c1
			-0.6f,  0.2f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,		// top vert   			#ebd2c1     14
			// mans right arm
			//* (11)													// bottom-right vert	#ebd2c1
			 0.6f,  0.0f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,		// bottom-left vert   	#ebd2c1     15
			 0.6f,  0.2f,  0.0f,		0.92f, 0.82f, 0.76f, 1.0f,		// top vert   			#ebd2c1     16
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

		m_MansVA = Deya::VertexArray::Create();
		Deya::Ref<Deya::VertexBuffer> mansVB;
		mansVB.reset(Deya::VertexBuffer::Create(mansVerts, sizeof(mansVerts)));
		mansVB->SetLayout(layout);
		m_MansVA->AddVertexBuffer(mansVB);

		Deya::Ref<Deya::IndexBuffer> mansIB;
		mansIB.reset(Deya::IndexBuffer::Create(mansIndices, sizeof(mansIndices) / sizeof(uint32_t)));
		m_MansVA->SetIndexBuffer(mansIB);

		/**
		 * !Camellia
		 */

		float camelliaVerts[4 * 5] =	// 4 verts * (3 dimensions + 2 tex coords)
		{
			// POS					TEX COORDS
			 0.5f,  0.5f, 0.0f, 	1.0f, 1.0f,		// top right
			-0.5f,  0.5f, 0.0f, 	0.0f, 1.0f,		// top left
			 0.5f, -0.5f, 0.0f, 	1.0f, 0.0f,		// bottom right
			-0.5f, -0.5f, 0.0f,  	0.0f, 0.0f		// bottom left
		};

		uint32_t camelliaIndices[6]
		{
			3, 0, 1,
			3, 2, 0
		};

		m_CamelliaVA = Deya::VertexArray::Create();
		Deya::Ref<Deya::VertexBuffer> camelliaVB;
		camelliaVB.reset(Deya::VertexBuffer::Create(camelliaVerts, sizeof(camelliaVerts)));
		camelliaVB->SetLayout(layoutTexture);
		m_CamelliaVA->AddVertexBuffer(camelliaVB);

		Deya::Ref<Deya::IndexBuffer> camelliaIB;
		camelliaIB.reset(Deya::IndexBuffer::Create(camelliaIndices, sizeof(camelliaIndices) / sizeof(uint32_t)));
		m_CamelliaVA->SetIndexBuffer(camelliaIB);

		UpdateShaders();

		m_CamelliaTexture = Deya::Texture2D::Create("Sandbox/assets/textures/camellia-face-transparent.png"); // FIXME: Fix paths (remove need for 'Sandbox/')

		std::dynamic_pointer_cast<Deya::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Deya::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Deya::Timestep ts) override
    {
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
        Deya::RenderCommand::SetClearColor(m_BackgroundColour);
		Deya::RenderCommand::Clear();

        /**
         * ! Render Flow
         */

		Deya::Renderer::BeginScene(m_CameraController.GetCamera());

		auto flatColourShader = m_ShaderLibrary.Get("FlatColourShader");

		std::dynamic_pointer_cast<Deya::OpenGLShader>(flatColourShader)->Bind();

        if (m_RenderLegume)
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

            glm::vec4 redColour(0.8f, 0.2f, 0.3f, 0.0f);    // #cc334c
            glm::vec4 blueColour(0.3f, 0.2f, 0.8f, 0.0f);   // #4c33cc
            for (int i = 0; i < 40; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    glm::vec3 pos(i * 0.15f - 1.5f, j * 0.15f - 1.5f, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                    if (i % 2 == 0)
                        std::dynamic_pointer_cast<Deya::OpenGLShader>(flatColourShader)->UploadUniformFloat4("u_Colour", m_LegumeColour1);
                    else
                        std::dynamic_pointer_cast<Deya::OpenGLShader>(flatColourShader)->UploadUniformFloat4("u_Colour", m_LegumeColour2);
                
                    Deya::Renderer::Submit(flatColourShader, m_LegumeVA, transform);
                }
            }   
        }
        if (m_RenderMans)
        {
            Deya::Renderer::Submit(m_Shader, m_MansVA);
        }
		if (m_RenderCamellia)
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.57f, 0.0f)) * scale;

			m_CamelliaTexture->Bind();
			Deya::Renderer::Submit(m_TextureShader, m_CamelliaVA, transform);
		}

		Deya::Renderer::EndScene();
    }
    
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");

        if (ImGui::CollapsingHeader("Colours"))
        {
            ImGui::ColorPicker4("BG Colour", glm::value_ptr(m_BackgroundColour));

            ImGui::ColorPicker4("Legume Colour 1", glm::value_ptr(m_LegumeColour1));
            ImGui::ColorPicker4("Legume Colour 2", glm::value_ptr(m_LegumeColour2));
        }

        if (ImGui::CollapsingHeader("Object Settings"))
        {
            ImGui::Checkbox("Render Legume?", &m_RenderLegume);
            ImGui::Checkbox("Render Mans?", &m_RenderMans);
            ImGui::Checkbox("Render Camellia?", &m_RenderCamellia);
        }

		if (ImGui::CollapsingHeader("Shaders"))
		{
			bool isPressed = ImGui::Button("Update Shaders");
			if (isPressed)
				UpdateShaders();
		}   

        ImGui::End();
    }

    void OnEvent(Deya::Event& e) override
    {
		m_CameraController.OnEvent(e);
	}
private:
	void UpdateShaders()
	{
		DY_CORE_INFO("Updating shaders...");
        m_ShaderLibrary.Load("Sandbox/assets/shaders/FlatColourShader.glsl");
		m_Shader = Deya::Shader::Create("Sandbox/assets/shaders/LayoutColourShader.glsl");
		m_TextureShader = Deya::Shader::Create("Sandbox/assets/shaders/TextureShader.glsl");
	}
	Deya::ShaderLibrary m_ShaderLibrary;
    Deya::Ref<Deya::Shader> m_Shader;
    //Deya::Ref<Deya::Shader> m_FlatColourShader;
    Deya::Ref<Deya::Shader> m_TextureShader;

    Deya::Ref<Deya::VertexArray> m_LegumeVA;
    Deya::Ref<Deya::VertexArray> m_MansVA;
	Deya::Ref<Deya::VertexArray> m_CamelliaVA;

	Deya::Ref<Deya::Texture2D> m_CamelliaTexture;

    Deya::Ref<Deya::VertexBuffer> m_VertexBuffer;
    Deya::Ref<Deya::IndexBuffer> m_IndexBuffer;

    Deya::OrthographicCameraController m_CameraController;

    glm::vec4 m_BackgroundColour;
    glm::vec4 m_LegumeColour1;
    glm::vec4 m_LegumeColour2;

    bool m_RenderLegume = true;
    bool m_RenderMans = true;
    bool m_RenderCamellia = true;
};

class Sandbox : public Deya::Application
{
public:
    Sandbox()
    {
        // PushLayer(new ExampleLayer);
		PushLayer(new Sandbox2D());
    }

    ~Sandbox() {}
};

Deya::Application* Deya::CreateApplication()
{
    return new Sandbox();
}