#pragma once

#include "Deya/Core.h"
#include "Deya/Events/Event.h"

namespace Deya 
{
    class DEYA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}