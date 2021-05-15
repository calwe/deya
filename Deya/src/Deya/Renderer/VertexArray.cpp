#include "dypch.h"
#include "Deya/Renderer/VertexArray.h"
#include "Deya/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Deya
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
        }

        DY_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}