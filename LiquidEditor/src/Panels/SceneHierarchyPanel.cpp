#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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
            m_SelectionContext = {};

        // Right-click on *blank space*
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty"))
                m_Context->CreateEntity("Empty Entity");

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");

            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Camera"))
                {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Sprite Renderer"))
                {
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();   
            }
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

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Empty"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.21f, 0.15f, 0.25f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.26f, 0.2f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.21f, 0.15f, 0.25f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f, 0.11f, 0.16f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, { 0.2f, 0.16f, 0.21f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, { 0.15f, 0.11f, 0.16f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(6);

        ImGui::Columns(1);

        ImGui::PopID();
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

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        if (entity.HasComponent<TransformComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
            
            if (open)
            {
                auto& tc = entity.GetComponent<TransformComponent>();
                DrawVec3Control("Position", tc.Translation);

                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);

                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<CameraComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            bool open = ImGui::TreeNodeEx((void*) typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("^", ImVec2{ 20, 20 }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) 
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

            if (removeComponent)
                entity.RemoveComponent<CameraComponent>();
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            bool open = ImGui::TreeNodeEx((void*) typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("^", ImVec2{ 20, 20 }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) 
            {
                // REMOVE: Temporary!
                auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
                auto& colour = spriteRendererComponent.Colour;

                ImGui::ColorEdit4("Colour", glm::value_ptr(colour));

                ImGui::TreePop(); 
            }

            if (removeComponent)
                entity.RemoveComponent<SpriteRendererComponent>();
        }

        if (entity.HasComponent<NativeScriptComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(NativeScriptComponent).hash_code(), treeNodeFlags, "Native Script")) { ImGui::TreePop(); }
        }
    }
}