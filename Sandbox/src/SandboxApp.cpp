#include <Deya.h>

#include "imgui/imgui.h"

class ExampleLayer : public Deya::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), 
            m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), 
            m_BackgroundColour({0.1f, 0.1f, 0.1f, 1.0f})
    {
        m_VertexArray.reset(Deya::VertexArray::Create());
	
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
	
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer.reset(Deya::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_MansVA.reset(Deya::VertexArray::Create());
		std::shared_ptr<Deya::VertexBuffer> mansVB;
		mansVB.reset(Deya::VertexBuffer::Create(mansVerts, sizeof(mansVerts)));
		mansVB->SetLayout(layout);
		m_MansVA->AddVertexBuffer(mansVB);

		std::shared_ptr<Deya::IndexBuffer> mansIB;
		mansIB.reset(Deya::IndexBuffer::Create(mansIndices, sizeof(mansIndices) / sizeof(uint32_t)));
		m_MansVA->SetIndexBuffer(mansIB);

		/**
		 * !Shaders!
		 */

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position + 0.5, 1.0);
			}
		)";

		m_ShaderMoved.reset(new Deya::Shader(vertexSrcMoved, fragmentSrc));
		m_Shader.reset(new Deya::Shader(vertexSrc, fragmentSrc));
    }

    void OnUpdate(Deya::Timestep ts) override
    {
        // DY_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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

		Deya::Renderer::BeginScene(m_Camera);

		Deya::Renderer::Submit(m_Shader, m_MansVA);
		Deya::Renderer::Submit(m_ShaderMoved, m_VertexArray);

		Deya::Renderer::EndScene();
    }
    
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorPicker4("BG Colour", (float*) &m_BackgroundColour);
        ImGui::End();
    }

    void OnEvent(Deya::Event& event) override
    {
    }
private:
    std::shared_ptr<Deya::Shader> m_Shader;
    std::shared_ptr<Deya::Shader> m_ShaderMoved;

    std::shared_ptr<Deya::VertexArray> m_VertexArray;
    std::shared_ptr<Deya::VertexArray> m_MansVA;

    std::shared_ptr<Deya::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Deya::IndexBuffer> m_IndexBuffer;

    Deya::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 3.0f;
    
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 100.0f;

    glm::vec4 m_BackgroundColour;
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