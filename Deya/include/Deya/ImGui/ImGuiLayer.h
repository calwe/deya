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

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& e);
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);

        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);

        bool OnWindowResizedEvent(WindowResizeEvent& e);
    private:
        float m_Time = 0.0f;
    };
}