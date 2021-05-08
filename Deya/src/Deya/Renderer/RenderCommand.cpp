#include "dypch.h"
#include "Deya/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Deya
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI; 
}