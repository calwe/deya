#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    
}

void Sandbox2D::OnDetach() {}
    
void Sandbox2D::OnUpdate(Deya::Timestep ts)
{
    // update
    m_CameraController.OnUpdate(ts);

    // render
    Deya::RenderCommand::SetClearColor(m_BackgroundColour);
    Deya::RenderCommand::Clear();

    Deya::Renderer2D::BeginScene(m_CameraController.GetCamera());
    //                          POSITION        SIZE            COLOUR
    Deya::Renderer2D::DrawQuad({ 0.25f, 0.25f }, { 1.0f, 1.0f }, m_SquareColour);
    Deya::Renderer2D::DrawQuad({ -0.25f, -0.25f }, { 1.0f, 1.0f }, m_Square2Colour);

    Deya::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

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