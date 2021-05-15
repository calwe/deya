#pragma once

extern Deya::Application* Deya::CreateApplication();

int main(int argc, char** argv)
{
    Deya::Log::Init();

    DY_PROFILE_BEGIN_SESSION("Startup", "DeyaProfile-Startup.json");
    auto app = Deya::CreateApplication();
    DY_PROFILE_END_SESSION();

    DY_PROFILE_BEGIN_SESSION("Runtime", "DeyaProfile-Runtime.json");
    app->Run();
    DY_PROFILE_END_SESSION();

    DY_PROFILE_BEGIN_SESSION("Shutdown", "DeyaProfile-Shutdown.json");
    delete app;
    DY_PROFILE_END_SESSION();
}