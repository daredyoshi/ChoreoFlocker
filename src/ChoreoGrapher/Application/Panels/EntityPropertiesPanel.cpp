
#include "EntityPropertiesPanel.h"
#include "Application/Scene/Components.h"
#include "Application/Scene/Entity.h"
#include "Application/Scene/SceneCamera.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"

namespace ChoreoGrapher{


    EntityPropertiesPanel::EntityPropertiesPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        setContext(scene); 
    }

    void EntityPropertiesPanel::setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        m_context = scene;;
    }

    void EntityPropertiesPanel::onImGuiRender(){
        ImGui::Begin("ChoreoGrapher::Properties");
        auto lastSelectedEntity = m_context->getLastSelectedEntity();
        auto test2 = lastSelectedEntity->getEntityHandle();
        auto test = *lastSelectedEntity;
        if(*lastSelectedEntity)
            drawEntityComponents(lastSelectedEntity); 
        ImGui::End();
    }

    void EntityPropertiesPanel::drawEntityComponents(ChoreoApp::Ref<ChoreoApp::Entity> entity){
        if(entity->hasComponent<ChoreoApp::TagComponent>()){
            auto& tag = entity->getComponent<ChoreoApp::TagComponent>().tag ;

            // if we go over this everythig will die
            char buffer[256];
            // initialize to 0
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))){
                tag = std::string(buffer);
            }
        }


        // Xform  
        if(entity->hasComponent<ChoreoApp::XformComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::XformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform") ){
                auto& xform = entity->getComponent<ChoreoApp::XformComponent>();
                
                ChoreoApp::XformKey& k { xform.xform->getKey(m_context->getTime()) };
                glm::vec3 p { k.getPosition() };
                if (ImGui::DragFloat3("Position", glm::value_ptr(p), 0.1f)){
                    k.setPosition(p);
                }
                glm::vec3 r { k.getEulerRotation() };
                if (ImGui::DragFloat3("Rotation", glm::value_ptr(r), 1.0f)){
                    k.setEulerRotation(r);
                }
                glm::vec3 s { k.getScale() };
                if (ImGui::DragFloat3("Scale", glm::value_ptr(s), 0.1f)){
                    k.setScale(s);
                }

                
                // if (ImGui::DragFloat3("Rotation", transform.getEulerRotation()))
                //
                //     camera.setPerspectiveVerticalFOV(glm::radians(perspFOV));


                ImGui::TreePop();
            }
        }

        // SPRITE RENDERER
        if(entity->hasComponent<ChoreoApp::SpriteRendererComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "SpriteRendererComponent") ){
                auto& color = entity->getComponent<ChoreoApp::SpriteRendererComponent>().color;
                ImGui::ColorEdit4("Color", glm::value_ptr(color));

                ImGui::TreePop();
            }
        }
        

        // CAMERA
        if(entity->hasComponent<ChoreoApp::CameraComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera") ){
                auto& cameraComponent= entity->getComponent<ChoreoApp::CameraComponent>();
                auto& camera = cameraComponent.camera;
                const char* projectiontypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionTypeString = projectiontypeStrings[(int)cameraComponent.camera.getProjectionType()];
                if (ImGui::BeginCombo("ProjectionType", currentProjectionTypeString)){
                    for (int i{0}; i<2; ++i){
                        // if this is the selected one
                        bool isSelected = currentProjectionTypeString == projectiontypeStrings[i];
                        // if clicked on it
                        if (ImGui::Selectable(projectiontypeStrings[i], isSelected)){
                            currentProjectionTypeString = projectiontypeStrings[i]; 
                            cameraComponent.camera.setProjectionType((ChoreoApp::SceneCamera::ProjectionType)i);
                        }
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    } 
                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Primary", &cameraComponent.primary);

                // PERSPECTIVE 
                if (camera.getProjectionType() == ChoreoApp::SceneCamera::ProjectionType::Perspective){
                    // float perspFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
                    // ImGui::InputDouble("Vertical FOV", 
                    const ChoreoApp::Scope<ChoreoApp::Time>& t {m_context->getTime()};
                    float perspectiveFOV{ glm::degrees(camera.getPerspectiveFOV()->getKey(t).getVal())};
                    if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV)){
                        camera.setPerspectiveFOV(glm::radians(perspectiveFOV), m_context->getTime());
                    }
                    float perspectiveNearClip{ camera.getPerspectiveNearClip()->getKey(t).getVal()};
                    if (ImGui::DragFloat("Vertical NearClip", &perspectiveNearClip)){
                        camera.setPerspectiveNearClip(perspectiveNearClip, m_context->getTime());
                    }
                    float perspectiveFarClip{ camera.getPerspectiveFarClip()->getKey(t).getVal()};
                    if (ImGui::DragFloat("Vertical FarClip", &perspectiveFarClip)){
                        camera.setPerspectiveFarClip(perspectiveFarClip, m_context->getTime());
                    }
                    //
                    // float perspNear = camera.getPerspectiveNearClip();
                    // if (ImGui::DragFloat("Near", &perspNear))
                    //     camera.setPerspectiveNearClip(perspNear);
                    //
                    // float perspFar = camera.getPerspectiveFarClip();
                    // if (ImGui::DragFloat("Far", &perspFar))
                    //     camera.setPerspectiveFarClip(perspFar);
                }

                // ORTHOGARPHIC
                if (camera.getProjectionType() == ChoreoApp::SceneCamera::ProjectionType::Orthographic){

                    // float orthoSize = camera.getOrthographicSize();
                    // if (ImGui::DragFloat("Size", &orthoSize))
                    //     camera.setOrthographicSize(orthoSize);
                    //
                    // float orthoNear = camera.getOrthographicNearClip();
                    // if (ImGui::DragFloat("Near", &orthoNear))
                    //     camera.setOrthographicNearClip(orthoNear);
                    //
                    // float orthoFar = camera.getOrthographicFarClip();
                    // if (ImGui::DragFloat("Far", &orthoFar))
                    //     camera.setOrthographicFarClip(orthoFar);
                    //
                    // ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
                }

                ImGui::TreePop();
            }
        }
         
    }
}
