#pragma once

#include "Deya/Renderer/RenderCommand.h"

namespace Deya
{
    class Renderer
    {
    public:
        static void BeginScene(); // TODO: Camera and Environment
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}