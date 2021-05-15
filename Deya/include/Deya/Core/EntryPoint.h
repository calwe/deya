#pragma once

extern Deya::Application* Deya::CreateApplication();

int main(int argc, char** argv)
{
    Deya::Log::Init();

    auto app = Deya::CreateApplication();
    app->Run();
    delete app;
}