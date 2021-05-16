#include "Level.h"

void Level::Init()
{
    m_Hotdogs.resize(4);
    for (int i = 0; i < 4; i++)
    {
        m_Hotdogs[i].Init(i);
    }
    m_Player.Init(&m_Hotdogs);

    m_BGTexture = Deya::Texture2D::Create("assets/textures/bg.png");
}

void Level::OnRender()
{
    Deya::Renderer2D::DrawQuad({0.0f, 0.0f, -0.5f}, {3.56f, 2.0f}, m_BGTexture);

    for (int i = 0; i < 4; i++)
    {
        m_Hotdogs[i].OnRender();
    }
    m_Player.OnRender();
}

void Level::OnUpdate(Deya::Timestep ts)
{
    for (int i = 0; i < 4; i++)
    {
        m_Hotdogs[i].OnUpdate(ts);
    }
    m_Player.OnUpdate(ts);
}