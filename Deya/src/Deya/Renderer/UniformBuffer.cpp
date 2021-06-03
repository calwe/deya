#include "dypch.h"

#include "Deya/Renderer/UniformBuffer.h"

#include "Deya/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Deya
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: DY_CORE_ASSERT_STRING(false, "RendererAPI::None not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }
}
