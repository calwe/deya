#include "dypch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Deya
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle) 
    {
        DY_CORE_ASSERT_STRING(windowHandle, "Window handle is null")
    }

    void OpenGLContext::Init()
    {
        DY_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        DY_CORE_ASSERT_STRING(status, "Failed to initialize glad");

        DY_CORE_INFO("OpenGL Info:");
        DY_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        DY_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        DY_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {   
        DY_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}