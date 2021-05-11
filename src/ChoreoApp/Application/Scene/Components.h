#pragma once

#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <memory>
#include <string>
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Controller.h"

namespace ChoreoApp{
    struct TagComponent{
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string&  _tag)
            : tag(_tag) {}

        operator std::string&() { return tag; }
    };

    struct XformComponent{
        // defaulting to eulers
        Ref<XformController> xform;

        // XformComponent() = default;
        XformComponent(const XformComponent&) = default;
        XformComponent(std::weak_ptr<Scene> scene, const std::string& label="No Name"){
            // XformComponent(scene, glm::mat4{0}, label);
            Scope<Time> t = CreateScope<Time>(0);
            xform =  {CreateRef<EulerXformController>(scene, label)};
        }

    };

    struct CameraComponent{
        SceneCamera camera;
        bool fixedAspectRatio{false};
        bool primary{true}; // TODO think about moving to scene

        // CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(std::weak_ptr<Scene> scene)
            : camera{scene} {}
    };

    struct NativeScriptComponent{
        ScriptableEntity* instance = nullptr;


        ScriptableEntity*(*instantiateScript)();
        void(*destoryScript)(NativeScriptComponent*);

        template<typename T>
        void bind(){
            instantiateScript = []() {return static_cast<ScriptableEntity*> ( new T() ); };
            destoryScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr;};
        }
    };

    struct SpriteRendererComponent{
        glm::vec4 color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& _color)
            : color(_color) {}
    };


}
