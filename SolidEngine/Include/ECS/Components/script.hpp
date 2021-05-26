#pragma once

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/script.sld.hpp"

namespace Solid SLDNamespace()
{
class SOLID_API SLDClass() Script : public Components
    {


    public:

        Script() = default;
        Script(GameObject* _go) {gameObject = _go;}
        virtual ~Script() = default;

		virtual void Init() {};
        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void LateUpdate() {};

        virtual void OnTriggerEnter(GameObject* _other) {};
        virtual void OnTriggerExit(GameObject* _other) {};
        virtual void OnContactEnter(GameObject* _other) {};
        virtual void OnContactExit(GameObject* _other) {};

    GameObject* GetGameObject(){return gameObject;}

		Script_GENERATED
    };
}// !namespace

File_GENERATED
