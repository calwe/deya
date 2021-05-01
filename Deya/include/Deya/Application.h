#pragma once

#include "dypch.h"

#include "Deya/Core.h"
#include "Deya/Events/Event.h"
#include "Deya/Window.h"

namespace Deya 
{
    class DEYA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}