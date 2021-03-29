#pragma once

namespace Solid
{
    class  Script
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

    };
}// !namespace
