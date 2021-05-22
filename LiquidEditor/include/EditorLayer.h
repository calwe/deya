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
        Ref<Texture2D> m_SpriteSheet;
        Ref<SubTexture2D> m_SpriteError;

        std::vector<ProfileResult> m_ProfileResults;

        glm::vec4 m_BackgroundColour = { 0.1f, 0.1f, 0.1f, 1.0f };
        glm::vec4 m_SquareColour = { 1.0f, 0.0f, 0.0f, 0.5f };
        glm::vec4 m_Square2Colour = { 0.0f, 0.0f, 1.0f, 0.5f };

        float m_Angle = 0.0f;

        uint32_t m_MapWidth, m_MapHeight;
        std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
    };
}