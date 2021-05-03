#include <Deya.h>

class ExampleLayer : public Deya::Layer
{
public:
    ExampleLayer()
        : Layer("Example") {}

    void OnUpdate() override
    {
        if (Deya::Input::IsKeyPressed(DY_KEY_TAB))
            DY_INFO("Tab key pressed");
    }

    void OnEvent(Deya::Event& event) override
    {
        //DY_TRACE("{0}", event);
    }
};

class Sandbox : public Deya::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer);
        PushOverlay(new Deya::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};

Deya::Application* Deya::CreateApplication()
{
    return new Sandbox();
}