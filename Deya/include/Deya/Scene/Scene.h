#pragma once

#include "entt.hpp"

namespace Deya
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
    private:
        entt::registry m_Registry;
    };
}
