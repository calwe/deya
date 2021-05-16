#pragma once

#include <Deya.h>

#include <vector>

#include "Player.h"
#include "Hotdog.h"

class Level
{
public:
    void Init();
    void OnRender();
    void OnUpdate(Deya::Timestep ts);

    Player GetPlayer() { return m_Player; }
private:
    Player m_Player;
    std::vector<Hotdog> m_Hotdogs;

    Deya::Ref<Deya::Texture2D> m_BGTexture;
};