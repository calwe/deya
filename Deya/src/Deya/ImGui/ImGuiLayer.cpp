#include "dypch.h"
#include "Deya/ImGui/ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Deya/Core/Application.h"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuizmo.h"

namespace Deya
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::OnAttach()
    {
        DY_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        io.Fonts->AddFontFromFileTTF("assets/fonts/nunito/Nunito-Bold.ttf", 18.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/nunito/Nunito-Regular.ttf", 18.0f);

        // Set Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak the WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColours();

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        DY_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e )
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        DY_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        DY_PROFILE_FUNCTION();  
        
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColours()
    {
        auto& colours = ImGui::GetStyle().Colors;
        colours[ImGuiCol_WindowBg] = ImVec4{ 0.14f, 0.13f, 0.15f, 1.0f };

        // headers
        colours[ImGuiCol_Header] = ImVec4{ 0.17f, 0.11f, 0.18f, 1.0f };            // mid
        colours[ImGuiCol_HeaderHovered] = ImVec4{ 0.21f, 0.15f, 0.22f, 1.0f };     // light
        colours[ImGuiCol_HeaderActive] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };      // dark

        // buttons
        colours[ImGuiCol_Button] = ImVec4{ 0.24f, 0.15f, 0.29f, 1.0f };            // mid
        colours[ImGuiCol_ButtonHovered] = ImVec4{ 0.21f, 0.15f, 0.22f, 1.0f };     // light
        colours[ImGuiCol_ButtonActive] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };      // dark

        // frame bg
        colours[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };            // mid
        colours[ImGuiCol_FrameBgHovered] = ImVec4{ 0.21f, 0.15f, 0.22f, 1.0f };     // light
        colours[ImGuiCol_FrameBgActive] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };      // dark

        // tabs
        colours[ImGuiCol_Tab] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };            // dark
        colours[ImGuiCol_TabHovered] = ImVec4{ 0.21f, 0.15f, 0.22f, 1.0f };     // light
        colours[ImGuiCol_TabActive] = ImVec4{ 0.17f, 0.11f, 0.18f, 1.0f };      // mid
        colours[ImGuiCol_TabUnfocused] = ImVec4{ 0.13f, 0.08f, 0.13f, 1.0f };      // dark
        colours[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.15f, 0.1f, 0.15f, 1.0f };      // mid-dark

        // title
        colours[ImGuiCol_TitleBg] = ImVec4{ 0.10f, 0.05f, 0.10f, 1.0f };            // dark
        colours[ImGuiCol_TitleBgActive] = ImVec4{ 0.10f, 0.05f, 0.10f, 1.0f };     // dark
        colours[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.93f, 0.75f, 0.93f, 1.0f };      // super light

        colours[ImGuiCol_MenuBarBg] = ImVec4{ 0.08f, 0.03f, 0.08f, 1.0f };
    }
}