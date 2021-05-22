#include <Deya.h>
#include <Deya/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Deya 
{
    class LiquidEditor : public Application
    {
    public:
        LiquidEditor()
            : Application("Liquid Editor")
        {
            PushLayer(new EditorLayer());
        }
        
        ~LiquidEditor() {}
    };

    Application* CreateApplication()
    {
        return new LiquidEditor();
    }
}