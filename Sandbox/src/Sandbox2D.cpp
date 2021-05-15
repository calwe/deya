#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    m_SquareVA = Deya::VertexArray::Create(); 

    float squareVertices[4 * 3] =	// 4 verts * (3 dimensions)
	{
		// POS					
		 0.5f,  0.5f, 0.0f, 	// top right
		-0.5f,  0.5f, 0.0f, 	// top left
		 0.5f, -0.5f, 0.0f, 	// bottom right
		-0.5f, -0.5f, 0.0f,  	// bottom left
	};

    uint32_t squareIndices[6]
	{
		3, 0, 1,
		3, 2, 0
	};

    Deya::BufferLayout layout =
	{
		{ Deya::ShaderDataType::Float3, "a_Position"}
	};

    Deya::Ref<Deya::VertexBuffer> squareVB;
    squareVB.reset(Deya::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout(layout);
    m_SquareVA->AddVertexBuffer(squareVB);

    Deya::Ref<Deya::IndexBuffer> sqaureIB;
    sqaureIB.reset(Deya::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(sqaureIB);

    m_FlatColourShader = Deya::Shader::Create("Sandbox/assets/shaders/FlatColourShader.glsl");
}

void Sandbox2D::OnDetach() {}
    
void Sandbox2D::OnUpdate(Deya::Timestep ts)
{
    // update
    m_CameraController.OnUpdate(ts);

    // render
    Deya::RenderCommand::SetClearColor(m_BackgroundColour);
    Deya::RenderCommand::Clear();

    Deya::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Deya::OpenGLShader>(m_FlatColourShader)->Bind(); // FIXME: make api agnostic
    std::dynamic_pointer_cast<Deya::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Colour", m_SquareColour);

    Deya::Renderer::Submit(m_FlatColourShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Deya::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Colours"))
    {
        ImGui::ColorPicker4("BG Colour", glm::value_ptr(m_BackgroundColour));

        ImGui::ColorPicker4("Square Colour", glm::value_ptr(m_SquareColour));
    }

    ImGui::End();
}

void Sandbox2D::OnEvent(Deya::Event& event)
{
    m_CameraController.OnEvent(event);
}