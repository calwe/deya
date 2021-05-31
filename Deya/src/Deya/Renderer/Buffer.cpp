#include "dypch.h"

#include "Deya/Renderer/Buffer.h"
#include "Deya/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGlBuffer.h"

namespace Deya
{
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT_STRING(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT_STRING(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(size);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT_STRING(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }
}