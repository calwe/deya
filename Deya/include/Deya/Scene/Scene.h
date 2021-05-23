#pragma once

#include "entt.hpp"

#include "Deya/Core/Timestep.h"
namespace Deya
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // tmp
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep ts);
    private:
        entt::registry m_Registry;
    };
}
