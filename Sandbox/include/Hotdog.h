#pragma once

#include <Deya.h>

class Hotdog
{
public:
    void Init(uint32_t index);
    void OnRender();
    void OnUpdate(Deya::Timestep ts);

    bool CheckCollisions(glm::vec3 playerPos);
private:
    void UpdateYPos();
private:
    Deya::Ref<Deya::Texture2D> m_HotdogTexture;

    float m_HotdogStackPositionY = 0.0f;
    float m_HotdogStackPositionX = 2.0f;
    float m_HotdogStackSpeed = 1.0f;
};