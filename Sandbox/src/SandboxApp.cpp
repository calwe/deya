#include <Deya.h>

class Sandbox : public Deya::Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }
};

Deya::Application* Deya::CreateApplication()
{
    return new Sandbox();
}