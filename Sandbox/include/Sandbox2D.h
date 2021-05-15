#pragma once

#include <Deya.h>

class Sandbox2D : public Deya::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Deya::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Deya::Event& event) override;
private:
    Deya::OrthographicCameraController m_CameraController;

    // REMOVE: Temporary
    Deya::Ref<Deya::VertexArray> m_SquareVA;
    Deya::Ref<Deya::Shader> m_FlatColourShader;

    glm::vec4 m_BackgroundColour = { 0.1f, 0.1f, 0.1f, 1.0f };
    glm::vec4 m_SquareColour = { 0.3f, 0.7f, 0.5f, 1.0f };      // #4cb27f
};