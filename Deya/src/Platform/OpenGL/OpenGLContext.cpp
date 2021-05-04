#include "dypch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Deya
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle) 
    {
        DY_CORE_ASSERT(windowHandle, "Window handle is null")
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        DY_CORE_ASSERT(status, "Failed to initialize glad");

        DY_CORE_INFO("OpenGL Info:");
        DY_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        DY_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        DY_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}