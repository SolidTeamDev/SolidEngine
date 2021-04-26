#pragma once

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/script.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() Script : public Components
    {
    protected:
        GameObject* gameobject= nullptr;
    public:

        Script() = default;
        Script(GameObject* _go) {gameobject = _go;}
        virtual ~Script() = default;

        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void LateUpdate() = 0;

        GameObject* GetGameObject(){return gameobject;}

		Script_GENERATED
    };
}// !namespace

File_GENERATED