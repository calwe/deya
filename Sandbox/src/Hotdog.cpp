#include "Hotdog.h"

#include <stdlib.h>

void Hotdog::Init(uint32_t index)
{
    UpdateYPos();

    float xOffset = index * 1.0f;
    m_HotdogStackPositionX += xOffset;

    m_HotdogTexture = Deya::Texture2D::Create("assets/textures/hotdog.png");
}

void Hotdog::OnRender()
{
    Deya::Renderer2D::DrawQuad({m_HotdogStackPositionX, -1.2f + m_HotdogStackPositionY, 1.0f}, {0.069f * 3.0f, 0.599f * 3.0f}, m_HotdogTexture);
    Deya::Renderer2D::DrawRotatedQuad({m_HotdogStackPositionX, 1.2f + m_HotdogStackPositionY, 1.0f}, glm::radians(180.0f), {0.069f * 3.0f, 0.599f * 3.0f}, m_HotdogTexture);
}

void Hotdog::OnUpdate(Deya::Timestep ts)
{
    m_HotdogStackPositionX -= m_HotdogStackSpeed * ts;

    if (m_HotdogStackPositionX < -2.0f)
    {
        UpdateYPos();
        m_HotdogStackPositionX = 2.0f;
    }
}

bool Hotdog::CheckCollisions(glm::vec3 playerPos)
{
    if (m_HotdogStackPositionX < -0.1f || m_HotdogStackPositionX > 0.1f)
    {
        return false;
    }
    else if (playerPos.y > m_HotdogStackPositionY + 0.26f || playerPos.y < m_HotdogStackPositionY - 0.26f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Hotdog::UpdateYPos()
{
    m_HotdogStackPositionY = (float) (std::rand() % 100 - 50) / 100.0f;
}