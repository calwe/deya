#include "Level.h"

void Level::Init()
{
    m_Player.Init();

    m_BGTexture = Deya::Texture2D::Create("assets/textures/bg.png");
}

void Level::OnRender()
{
    Deya::Renderer2D::DrawQuad({0.0f, 0.0f, -0.5f}, {3.56f, 2.0f}, m_BGTexture);

    m_Player.OnRender();
}

void Level::OnUpdate(Deya::Timestep ts)
{
    m_Player.OnUpdate(ts);
}