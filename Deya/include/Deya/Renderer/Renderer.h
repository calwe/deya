#pragma once

#include "Deya/Renderer/RenderCommand.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/OrthographicCamera.h"

namespace Deya
{
    class Renderer
    {
    public:
        static void BeginScene(OrthographicCamera& camera); // TODO: Camera and Environment
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };
}