#pragma once

#include <Deya.h>

#include "Hotdog.h"

#include <glm/glm.hpp>

class Player
{
public:
    void Init(std::vector<Hotdog>* hotdog);
    void OnRender();
    void OnUpdate(Deya::Timestep ts);

    bool CheckIfDead();

    inline const glm::vec3& GetPosition() const { return m_PlayerPos; }
private:
    glm::vec3 m_PlayerPos = {0.0f, 0.0f, 0.0f};

    bool m_KeyDown = false;
    float m_Gravity = 0.05f;
    float m_FlapForce = 1.0f;
    float m_Velocity = 0.0f;

    bool m_IsDead = false;

    std::vector<Hotdog>* m_Hotdogs;
    
    Deya::Ref<Deya::Texture2D> m_MansTexture;
};