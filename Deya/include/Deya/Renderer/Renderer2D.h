#pragma once

#include "Deya/Renderer/OrthographicCamera.h"

namespace Deya
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour);
    };
}