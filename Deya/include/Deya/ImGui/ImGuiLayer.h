#pragma once

#include "Deya/Layer.h"

#include "Deya/Events/KeyEvent.h"
#include "Deya/Events/MouseEvent.h" 
#include "Deya/Events/ApplicationEvent.h" 

namespace Deya
{
    class DEYA_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };
}