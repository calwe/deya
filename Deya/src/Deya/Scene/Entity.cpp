#include "dypch.h"

#include "Deya/Scene/Entity.h"

namespace Deya
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene) {}
}
