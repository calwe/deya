#pragma once

#if defined(DY_PLATFORM_LINUX) || defined(DY_PLATFORM_WINDOWS)

extern Deya::Application* Deya::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Deya::CreateApplication();
    app->Run();
    delete app;
}

#endif