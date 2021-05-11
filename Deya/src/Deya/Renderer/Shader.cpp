#include "dypch.h"

#include "Deya/Renderer/Shader.h"

#include "Deya/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Deya
{
    Shader* Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(filepath);
        }

        DY_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                DY_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        DY_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}