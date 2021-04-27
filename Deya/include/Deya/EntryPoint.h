#pragma once

#if defined(DY_PLATFORM_LINUX) || defined(DY_PLATFORM_WINDOWS)

extern Deya::Application* Deya::CreateApplication();

int main(int argc, char** argv)
{
    Deya::Log::Init();
    DY_CORE_TRACE("1");
    DY_CORE_INFO("2");
    DY_CORE_WARN("3");
    DY_CORE_ERROR("4");
    DY_CORE_FATAL("5");

    DY_TRACE("1");
    DY_INFO("2");
    DY_WARN("3");
    DY_ERROR("4");
    DY_FATAL("5");

    auto app = Deya::CreateApplication();
    app->Run();
    delete app;
}

#endif