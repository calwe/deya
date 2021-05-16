#include <Deya.h>
#include <Deya/Core/EntryPoint.h>

class Application : public Deya::Application
{
public:
    Application()
    {
		
    }

    ~Application() {}
};

Deya::Application* Deya::CreateApplication()
{
    return new Application();
}