#pragma once

#include <Deya.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Deya/Renderer/EditorCamera.h"

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
        bool OnKeyPressed(KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveSceneAs();
    private:
        OrthographicCameraController m_CameraController;

        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SqaureEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;

        bool m_PrimaryCamera = false;
        
        EditorCamera m_EditorCamera;

        Ref<Texture2D> m_MansTexture;
        Ref<Texture2D> m_MansSlimTexture;

        glm::vec4 m_BackgroundColour = { 0.09f, 0.08f, 0.1f, 1.0f };

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        int m_GizmoType = -1;

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}
