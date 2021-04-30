#pragma once

#include "Core/Maths/Vector/vector3.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/light.sld.hpp"

namespace Solid SLDNamespace()
{
    enum class ELightType
    {
        DIRECTIONAL,
        SPOT,
        POINT
    };

    class SOLID_API SLDClass() Light : public Components
    {
    private:
    public:

        ELightType type = ELightType::DIRECTIONAL;
        Vec3i color = {1,1,1};
        Vec3  intensity = 1;

        Light() = default;
        ~Light() override = default;

        void Init() override {};

        Light_GENERATED
    };
} //!namespace

File_GENERATED