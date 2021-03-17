#pragma once

namespace Solid
{
    class  Script
    {
    public:
        Script() = default;
        virtual ~Script() = default;

        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void LateUpdate() = 0;

    };
}// !namespace
