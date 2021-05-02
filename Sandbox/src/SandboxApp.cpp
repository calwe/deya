#include <Deya.h>

class ExampleLayer : public Deya::Layer
{
public:
    ExampleLayer()
        : Layer("Example") {}

    void OnUpdate() override
    {
        DY_INFO("ExampleLayer::Update");
    }

    void OnEvent(Deya::Event& event) override
    {
        DY_TRACE("{0}", event);
    }
};

class Sandbox : public Deya::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer);
    }

    ~Sandbox()
    {

    }
};

Deya::Application* Deya::CreateApplication()
{
    return new Sandbox();
}