#pragma once

#include "Deya/Scene/Scene.h"

#include "entt.hpp"

namespace Deya
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            DY_CORE_ASSERT_STRING(!HasComponent<T>(), "Entity already has component");

            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            DY_CORE_ASSERT_STRING(HasComponent<T>(), "Entity doesn't has component");

            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            DY_CORE_ASSERT_STRING(HasComponent<T>(), "Entity doesn't has component");

            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t) m_EntityHandle; }
        operator entt::entity() const { return m_EntityHandle; }

        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !operator==(other); }
    private: 
        entt::entity m_EntityHandle { entt::null };
        Scene* m_Scene = nullptr;
    };
}
