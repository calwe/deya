#pragma once

#include "entt.hpp"

#include "Deya/Core/Timestep.h"
namespace Deya
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        // tmp
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep ts);
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}
