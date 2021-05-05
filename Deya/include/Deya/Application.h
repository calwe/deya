#pragma once

#include "Deya/Core.h"

#include "Deya/LayerStack.h"
#include "Deya/Events/Event.h"
#include "Deya/Window.h"
#include "Deya/Events/ApplicationEvent.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/Buffer.h"
#include "Deya/ImGui/ImGuiLayer.h"

namespace Deya 
{
    class DEYA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        uint32_t m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}