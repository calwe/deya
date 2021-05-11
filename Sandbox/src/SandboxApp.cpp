#include <Deya.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Deya::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), 
            m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), 
            m_BackgroundColour({0.1f, 0.1f, 0.1f, 0.0f}),
            m_LegumeColour1({0.8f, 0.2f, 0.3f, 0.0f}),
            m_LegumeColour2({0.3f, 0.2f, 0.8f, 0.0f})
    {
        m_LegumeVA.reset(Deya::VertexArray::Create());
	
		/**
		 * !Legume!
		 */
		
		// legume verts
		float vertices[6 * 7] = // 6 verts * (3 dimensions + 4 colour channels)
		{
			// middle of legume
			// POS						COLOUR
			-0.125f,  0.2f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // top left
			-0.125f, -0.2f, 0.0f, 		0.25f, 0.13f, 0.03f, 0.0f, // bottom left
			 0.125f,  0.2f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // top right
			 0.125f, -0.2f, 0.0f,		0.25f, 0.13f, 0.03f, 0.0f, // bottom right

			// top and bottom verts
			 0.0f,  0.35f, 0.0f,		0.35f, 0.23f, 0.05f, 0.0f, // top
			 0.0f, -0.35f, 0.0f,		0.15f, 0.03f, 0.01f, 0.0f  // bottom
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

		Deya::BufferLayout layoutTexture =
		{
			{ Deya::ShaderDataType::Float3, "a_Position" },
			{ Deya::ShaderDataType::Float2, "a_TexCoord" },
		};
	
		m_VertexBuffer->SetLayout(layout);
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

		m_MansVA.reset(Deya::VertexArray::Create());
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

		m_CamelliaVA.reset(Deya::VertexArray::Create());
		Deya::Ref<Deya::VertexBuffer> camelliaVB;
		camelliaVB.reset(Deya::VertexBuffer::Create(camelliaVerts, sizeof(camelliaVerts)));
		camelliaVB->SetLayout(layoutTexture);
		m_CamelliaVA->AddVertexBuffer(camelliaVB);

		Deya::Ref<Deya::IndexBuffer> camelliaIB;
		camelliaIB.reset(Deya::IndexBuffer::Create(camelliaIndices, sizeof(camelliaIndices) / sizeof(uint32_t)));
		m_CamelliaVA->SetIndexBuffer(camelliaIB);

		/**
		 * !Shaders!
		 */

		std::string vertexSrc = R"glsl(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)glsl";

		std::string fragmentSrc = R"glsl(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)glsl";

        std::string flatColourFragmentSrc = R"glsl(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
            uniform vec4 u_Colour;

			void main()
			{
				color = u_Colour;
			}
		)glsl";

		/** 
		 * !Texture Shader
		 */

		std::string textureShaderVertexSrc = R"glsl(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)glsl";

		std::string textureShaderFragmentSrc = R"glsl(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
            uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)glsl";

        m_FlatColourShader.reset(Deya::Shader::Create(vertexSrc, flatColourFragmentSrc));
		m_Shader.reset(Deya::Shader::Create(vertexSrc, fragmentSrc));
		m_TextureShader.reset(Deya::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_CamelliaTexture = Deya::Texture2D::Create("Sandbox/assets/textures/camellia-face.png"); // TODO: Does this work on Windows?

		std::dynamic_pointer_cast<Deya::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Deya::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Deya::Timestep ts) override
    {
        // DY_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        //* camera
        if (Deya::Input::IsKeyPressed(DY_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Deya::Input::IsKeyPressed(DY_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Deya::Input::IsKeyPressed(DY_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Deya::Input::IsKeyPressed(DY_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Deya::Input::IsKeyPressed(DY_KEY_Q))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        else if (Deya::Input::IsKeyPressed(DY_KEY_E))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        Deya::RenderCommand::SetClearColor(m_BackgroundColour);
		Deya::RenderCommand::Clear();

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);

        /**
         * ! Render Flow
         */

		Deya::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Deya::OpenGLShader>(m_FlatColourShader)->Bind();

        if (m_RenderLegume)
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

            glm::vec4 redColour(0.8f, 0.2f, 0.3f, 1.0f);    // #cc334c
            glm::vec4 blueColour(0.3f, 0.2f, 0.8f, 1.0f);   // #4c33cc
            for (int i = 0; i < 40; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    glm::vec3 pos(i * 0.15f - 1.5f, j * 0.15f - 1.5f, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                    if (i % 2 == 0)
                        std::dynamic_pointer_cast<Deya::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Colour", m_LegumeColour1);
                    else
                        std::dynamic_pointer_cast<Deya::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Colour", m_LegumeColour2);
                
                    Deya::Renderer::Submit(m_FlatColourShader, m_LegumeVA, transform);
                }
            }   
        }
        if (m_RenderMans)
        {
            Deya::Renderer::Submit(m_Shader, m_MansVA);
        }
		if (m_RenderCamellia)
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.6f, 0.0f)) * scale;

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

        ImGui::End();
    }

    void OnEvent(Deya::Event& event) override
    {
    }
private:
    Deya::Ref<Deya::Shader> m_Shader;
    Deya::Ref<Deya::Shader> m_FlatColourShader;
    Deya::Ref<Deya::Shader> m_TextureShader;

    Deya::Ref<Deya::VertexArray> m_LegumeVA;
    Deya::Ref<Deya::VertexArray> m_MansVA;
	Deya::Ref<Deya::VertexArray> m_CamelliaVA;

	Deya::Ref<Deya::Texture2D> m_CamelliaTexture;

    Deya::Ref<Deya::VertexBuffer> m_VertexBuffer;
    Deya::Ref<Deya::IndexBuffer> m_IndexBuffer;

    Deya::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 3.0f;
    
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 100.0f;

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
        PushLayer(new ExampleLayer);
    }

    ~Sandbox()
    {

    }
};

Deya::Application* Deya::CreateApplication()
{
    return new Sandbox();
}