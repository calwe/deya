#pragma once

#include <Deya.h>

#include "Player.h"

class Level
{
public:
    void Init();
    void OnRender();
    void OnUpdate(Deya::Timestep ts);
private:
    Player m_Player;

    Deya::Ref<Deya::Texture2D> m_BGTexture;
};