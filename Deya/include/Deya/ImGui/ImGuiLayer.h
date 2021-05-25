#pragma once

#include "Deya/Core/Layer.h"

#include "Deya/Events/KeyEvent.h"
#include "Deya/Events/MouseEvent.h" 
#include "Deya/Events/ApplicationEvent.h" 

#include <glm/glm.hpp>

namespace Deya
{
    class DEYA_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }
        void SetDarkThemeColours();
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };
}