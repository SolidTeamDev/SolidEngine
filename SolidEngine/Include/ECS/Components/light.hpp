#pragma once

#include "Core/Maths/Vector/vector3.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/light.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() Light : public Components
    {
    private:
        static std::vector<Light> lightList;
    public:

        enum ELightType
        {
            DIRECTIONAL,
            SPOT,
            POINT
        };

        ELightType type = ELightType::DIRECTIONAL;
        Vec3 color = {1,1,1};
        float  intensity = 1;

        Light() = default;
        ~Light() override = default;

        void Init() override;
        void Release() override;

        Light_GENERATED
    };
} //!namespace

File_GENERATED