#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    DY_PROFILE_FUNCTION();

    m_MansTexture = Deya::Texture2D::Create("assets/textures/mans.png");
    m_MansSlimTexture = Deya::Texture2D::Create("assets/textures/mans_slim.png");
}

void Sandbox2D::OnDetach() { DY_PROFILE_FUNCTION(); }
    
void Sandbox2D::OnUpdate(Deya::Timestep ts)
{
    DY_PROFILE_FUNCTION();

    // update
    m_CameraController.OnUpdate(ts);
    m_Angle += 0.5f;

    // render
    Deya::Renderer2D::ResetStats();
    {
        DY_PROFILE_SCOPE("Renderer Clear");
        Deya::RenderCommand::SetClearColor(m_BackgroundColour);
        Deya::RenderCommand::Clear();
    }

    {
        DY_PROFILE_SCOPE("Renderer Draw");
        Deya::Renderer2D::BeginScene(m_CameraController.GetCamera());
        
        Deya::Renderer2D::DrawQuad({ 0.25f, 0.25f }, { 1.0f, 1.0f }, m_SquareColour);
        Deya::Renderer2D::DrawQuad({ -0.25f, -0.25f }, { 1.0f, 1.0f }, m_Square2Colour);

        Deya::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f }, glm::radians(m_Angle), { 0.5f, 0.5f }, m_MansTexture);
        Deya::Renderer2D::DrawQuad({ 0.6f, 0.0f, 0.1f }, { 0.5f, 0.5f }, m_MansSlimTexture);

        Deya::Renderer2D::EndScene();

        // Stress test
        Deya::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -10.0f; y < 10.0f; y += 0.5f)
        {
            for (float x = -10.0f; x < 10.0f; x += 0.1f)
            {
                glm::vec4 colour = { (x + 10.0f) / 20.0f, 0.4f, (y + 10.0f) / 20.0f, 1.0f };
                Deya::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, colour);
            }
        }
        Deya::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    DY_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Renderer2D Stats"))
    {
        auto stats = Deya::Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    }

    if (ImGui::CollapsingHeader("Colours"))
    {
        ImGui::ColorPicker4("BG Colour", glm::value_ptr(m_BackgroundColour));

        ImGui::ColorPicker4("Square Colour", glm::value_ptr(m_SquareColour));
        ImGui::ColorPicker4("Square 2 Colour", glm::value_ptr(m_Square2Colour));
    }

    ImGui::End();
}

void Sandbox2D::OnEvent(Deya::Event& event)
{
    m_CameraController.OnEvent(event);
}