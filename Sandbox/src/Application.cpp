#include <Deya.h>
#include <Deya/Core/EntryPoint.h>

#include "GameLayer.h"

class FlappyMansApp : public Deya::Application
{
public:
    FlappyMansApp()
    {
        // Push our GameLayer onto the layer stack
		PushLayer(new GameLayer());
    }

    ~FlappyMansApp() {}
};

Deya::Application* Deya::CreateApplication()
{
    return new FlappyMansApp();
}