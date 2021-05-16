#include "GameLayer.h"

#include "Player.h"

GameLayer::GameLayer()
    : Layer("GameLayer") /* Create our layer */
{
    auto& window = Deya::Application::Get().GetWindow(); // Get our current window
    
    /* Setup our camera */
    CreateCamera(window.GetWidth(), window.GetHeight());
}

void GameLayer::OnAttach()
{
    m_Level.Init();
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(Deya::Timestep ts)
{
    if (Deya::Input::IsKeyPressed(DY_KEY_SPACE))
    {
        m_Playing = true;
    }

    if (m_Playing)
    {
        if (!m_Level.GetPlayer().CheckIfDead())
            m_Level.OnUpdate(ts);
    }

    // Clear our scene with the colour #212121
    Deya::RenderCommand::SetClearColor({0.13f, 0.13f, 0.13f, 1.0f});
    Deya::RenderCommand::Clear();

    /* Render Flow */

    // Begin our scene for rendering (sets VP matrix, etc..)
    Deya::Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    Deya::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender() {};

void GameLayer::OnEvent(Deya::Event& event)
{
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    float aspectRatio  = (float) width / (float) height; // Get aspect ration

    float camWidth = 1.0f; // width of camera (zoom level)
    // Dimension for camera
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    // Create our camera
    m_Camera = Deya::CreateScope<Deya::OrthographicCamera>(left, right, bottom, top);
}