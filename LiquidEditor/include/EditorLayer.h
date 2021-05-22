#pragma once

#include <Deya.h>

namespace Deya
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;
    private:
        OrthographicCameraController m_CameraController;

        Ref<Framebuffer> m_Framebuffer;

        Ref<Texture2D> m_MansTexture;
        Ref<Texture2D> m_MansSlimTexture;

        glm::vec4 m_BackgroundColour = { 0.1f, 0.1f, 0.1f, 1.0f };
        glm::vec4 m_SquareColour = { 1.0f, 0.0f, 0.0f, 1.0f };
        glm::vec4 m_Square2Colour = { 0.0f, 0.0f, 1.0f, 1.0f };

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        float m_Angle = 0.0f;
    };
}