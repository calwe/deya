#include "dypch.h"

#include "Deya/Application.h"

#include "Deya/Events/ApplicationEvent.h"
#include "Deya/Window.h"

#include <GLFW/glfw3.h>

namespace Deya
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.68, 0.55, 0.84, 1); // purple (#af8dd6)
            glClear(GL_COLOR_BUFFER_BIT);

            m_Window->OnUpdate();
        }
    }
}