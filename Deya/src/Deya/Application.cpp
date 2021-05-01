#include "dypch.h"

#include "Deya/Application.h"

#include "Deya/Events/ApplicationEvent.h"
#include "Deya/Log.h"

namespace Deya
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			DY_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			DY_TRACE(e);
		}

        while (true);
    }
}