#pragma once

#include <Deya.h>

#include "Level.h"

class GameLayer : public Deya::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Deya::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Deya::Event& event) override;
private:
    void CreateCamera(uint32_t width, uint32_t height);
private:
    Deya::Scope<Deya::OrthographicCamera> m_Camera;
    bool m_Playing = false;;
    Level m_Level;
};