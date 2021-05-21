#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

#define TEST_DRAW 1
#define PARTICLE_SYSTEM 0
#define MINI_GAME 0

#if MINI_GAME
// W = Water
// GRASS LAYOUT:
// T Y U
// G H J
// B N M 
// INNER:
// E R
// D F
static const uint32_t s_MapWidth = 24;
static const char* s_MapTile = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWTYYYYYYYUWWWWWWWW"
"WWWWWTYFHHHHHHHDYUWWWWWW"
"WWWWTFHHHHHHHHHHHDUWWWWW"
"WWWWGHHHHHHHHHHHHHJWWWWW"
"WWWWGHHHHHHHHHHHHHJWWWWW"
"WWWWGHHHHHHHHHHHHHJWWWWW"
"WWWWGHHHHHHHHHHHHHJWWWWW"
"WWWWGHHHHHHHHHHHHHJWWWWW"
"WWWWBRHHHHHHHHHHHEMWWWWW"
"WWWWWBNRHHHHHHHENMWWWWWW"
"WWWWWWWBNNNNNNNMWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";
#endif

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    DY_PROFILE_FUNCTION();

    m_MansTexture = Deya::Texture2D::Create("assets/textures/mans.png");
    m_MansSlimTexture = Deya::Texture2D::Create("assets/textures/mans_slim.png");
    m_SpriteSheet = Deya::Texture2D::Create("assets/textures/good-tilemap.png");

#if MINI_GAME
    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTile) / m_MapWidth;

    m_SpriteError = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 10, 3 }, {128, 128}); // 1 symbol thing

    s_TextureMap['W'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 15, 0 }, {128, 128}); // WATER

    s_TextureMap['T'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 11 }, {128, 128}); // GRASS TL
    s_TextureMap['Y'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, {128, 128}); // GRASS TC
    s_TextureMap['U'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 11 }, {128, 128}); // GRASS TR

    s_TextureMap['G'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 10 }, {128, 128}); // GRASS CL
    s_TextureMap['H'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 10 }, {128, 128}); // GRASS CC
    s_TextureMap['J'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 10 }, {128, 128}); // GRASS CR

    s_TextureMap['B'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 9 }, {128, 128}); // GRASS BL
    s_TextureMap['N'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 9 }, {128, 128}); // GRASS BC
    s_TextureMap['M'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 9 }, {128, 128}); // GRASS BR

    s_TextureMap['E'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 11 }, {128, 128}); // GRASS ITR
    s_TextureMap['R'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 9, 11 }, {128, 128}); // GRASS TTL
    s_TextureMap['D'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 10 }, {128, 128}); // GRASS TBR
    s_TextureMap['F'] = Deya::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 9, 10 }, {128, 128}); // GRASS TBL
#endif

    // Particle System Settings
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 3.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

    m_CameraController.SetZoomLevel(10.0f);
}

void Sandbox2D::OnDetach() { DY_PROFILE_FUNCTION(); }
    
void Sandbox2D::OnUpdate(Deya::Timestep ts)
{
    DY_PROFILE_FUNCTION();

    // update
    m_CameraController.OnUpdate(ts);
    m_Angle += 0.5f;

    // render
    Deya::Renderer2D::ResetStats();
    {
        DY_PROFILE_SCOPE("Renderer Clear");
        Deya::RenderCommand::SetClearColor(m_BackgroundColour);
        Deya::RenderCommand::Clear();
    }


#if TEST_DRAW
    {
        DY_PROFILE_SCOPE("Renderer Draw");
        Deya::Renderer2D::BeginScene(m_CameraController.GetCamera());
        
        Deya::Renderer2D::DrawQuad({ 0.25f, 0.25f }, { 1.0f, 1.0f }, m_SquareColour);
        Deya::Renderer2D::DrawQuad({ -0.25f, -0.25f }, { 1.0f, 1.0f }, m_Square2Colour);

        Deya::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f }, glm::radians(m_Angle), { 0.5f, 0.5f }, m_MansTexture);
        Deya::Renderer2D::DrawQuad({ 0.6f, 0.0f, 0.1f }, { 0.5f, 0.5f }, m_MansSlimTexture);

        Deya::Renderer2D::EndScene();
    }
#endif

#if PARTICLE_SYSTEM
    if (Deya::Input::IsMouseButtonPressed(DY_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Deya::Input::GetMousePosition();
		auto width = Deya::Application::Get().GetWindow().GetWidth();
		auto height = Deya::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 50; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif

#if MINI_GAME
    {
        DY_PROFILE_SCOPE("Render Game");
        Deya::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                Deya::Ref<Deya::SubTexture2D> texture = s_TextureMap['W'];
                Deya::Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), m_MapHeight - y - (m_MapHeight / 2.0f), 0.0f }, { 1.0f, 1.0f }, texture);
            }
        }

        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTile[x + y * m_MapWidth];
                Deya::Ref<Deya::SubTexture2D> texture;
                if (s_TextureMap.find(tileType) != s_TextureMap.end())
                    texture = s_TextureMap[tileType];
                else
                    texture = m_SpriteError;

                Deya::Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), m_MapHeight - y - (m_MapHeight / 2.0f), 0.5f }, { 1.0f, 1.0f }, texture);
            }
        }

        Deya::Renderer2D::EndScene();
    }
#endif
}

void Sandbox2D::OnImGuiRender()
{
    DY_PROFILE_FUNCTION();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Exit"))    Deya::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    /**
     * ! SETTING WINDOW
     * 
     * must be inside dockspace
     */

    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Renderer2D Stats"))
    {
        auto stats = Deya::Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    }

    if (ImGui::CollapsingHeader("Colours"))
    {
        ImGui::ColorPicker4("BG Colour", glm::value_ptr(m_BackgroundColour));

        ImGui::ColorPicker4("Square Colour", glm::value_ptr(m_SquareColour));
        ImGui::ColorPicker4("Square 2 Colour", glm::value_ptr(m_Square2Colour));
    }


    ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnEvent(Deya::Event& event)
{
    m_CameraController.OnEvent(event);
}