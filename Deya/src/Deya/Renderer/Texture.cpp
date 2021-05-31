#include "dypch.h"
#include "Deya/Renderer/Texture.h"

#include "Deya/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Deya
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT_STRING(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(width, height);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT_STRING(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(path);
        }

        DY_CORE_ASSERT_STRING(false, "Unknown RendererAPI");
        return nullptr;
    }
}