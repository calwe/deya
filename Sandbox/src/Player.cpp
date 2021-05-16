#include "Player.h"

void Player::Init()
{
    // Load our texture into m_MansTexture
    m_MansTexture = Deya::Texture2D::Create("assets/textures/mans.png");
}

void Player::OnRender()
{
    /**
     * Draw our sprite
     * 
     * We use a rotated quad as we would like to sometimes have our quad rotated.
     * Deya has its API split like this as calculating rotation matrices is 
     * relatively slow, so its good to give the option for static quads, as
     * most quads don't require rotation
     */
    Deya::Renderer2D::DrawRotatedQuad(m_PlayerPos, glm::radians(-90.0f), { 0.25f, 0.25f }, m_MansTexture); 
}

void Player::OnUpdate(Deya::Timestep ts)
{
    if (!CheckIfDead())
    {
        if (Deya::Input::IsKeyPressed(DY_KEY_SPACE))
        {
            if (!m_KeyDown)
            {
                m_KeyDown = true;
                m_Velocity -= m_FlapForce; 
            }
            else
            {
                m_Velocity += m_Gravity;
            }
        }
        else
        {
            m_Velocity += m_Gravity;
            m_KeyDown = false;
        }

        m_PlayerPos = m_PlayerPos - glm::vec3(0.0f, m_Velocity * (float) ts, 0.0f);
    }
}

bool Player::CheckIfDead()
{
    if (m_PlayerPos.y < -1.0f || m_PlayerPos.y > 1.0f)
    {
        return true;
    }

    return false;
}