#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Deya/Scene/Components.h"

namespace Deya
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity { entityID, m_Context.get() };
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        {
            m_SelectionContext = {};
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;                  // This means you need to click on the arrow to open the tree, because clicking on the row should select it
        if (m_SelectionContext == entity)   flags |= ImGuiTreeNodeFlags_Selected;   // Will highlight the entity in the hierarchy if it is slected 
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];   // ? do we want to support extended character sets?
            memset(buffer, 0, sizeof(buffer)); 
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Name", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (entity.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionTypeString = projectionTypeStrings[(int) camera.GetProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeStrings[i];
                            camera.SetProjectionType((SceneCamera::ProjectionType) i);
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch (camera.GetProjectionType())
                {
                    case SceneCamera::ProjectionType::Perspective:
                    {
                        float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV()); 
                        if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
                            camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

                        float nearClip = camera.GetPerspectiveNearClip(); 
                        if (ImGui::DragFloat("Near Clip", &nearClip))
                            camera.SetPerspectiveNearClip(nearClip);

                        float farClip = camera.GetPerspectiveFarClip(); 
                        if (ImGui::DragFloat("Far Clip", &farClip))
                            camera.SetPerspectiveFarClip(farClip);

                        break;
                    }
                    case SceneCamera::ProjectionType::Orthographic:
                    {
                        float orthoSize = camera.GetOrthographicSize(); 
                        if (ImGui::DragFloat("Size", &orthoSize))
                            camera.SetOrthographicSize(orthoSize);

                        float orthoNear = camera.GetOrthographicNearClip(); 
                        if (ImGui::DragFloat("Near Clip", &orthoNear))
                            camera.SetOrthographicNearClip(orthoNear);

                        float orthoFar = camera.GetOrthographicFarClip(); 
                        if (ImGui::DragFloat("Far Clip", &orthoFar))
                            camera.SetOrthographicFarClip(orthoFar);

                        break;
                    }
                }
            
                ImGui::TreePop(); 
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) { ImGui::TreePop(); }
        }

        if (entity.HasComponent<NativeScriptComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Native Script")) { ImGui::TreePop(); }
        }
    }
}