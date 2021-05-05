#include "dypch.h"

#include "Deya/Renderer/Buffer.h"
#include "Deya/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGlBuffer.h"

namespace Deya
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
                DY_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
        }

        DY_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
                DY_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLIndexBuffer(indices, size);
        }

        DY_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}