#pragma once

#include "Deya/Layer.h"

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
        virtual void OnImGuiRender() override;

        inline const float GetCameraRotation() const { return m_CameraRotation; }
        inline const glm::vec3& GetCameraPosition() const { return m_CameraPosition; }

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;

        float m_CameraRotation = 0.0f;
        glm::vec3 m_CameraPosition;
    };
}