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

        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void LateUpdate() = 0;

        GameObject* GetGameObject(){return gameObject;}

		Script_GENERATED
    };
}// !namespace

File_GENERATED