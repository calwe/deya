#pragma once

#include "Scene.h"

namespace Deya
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& filepath);            // Serialize to yaml
        void SerializeRuntime(const std::string& filepath);     // Serialize to binary

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Ref<Scene> m_Scene;
    };
}
