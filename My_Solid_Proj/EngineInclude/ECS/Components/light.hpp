#pragma once

#include "Core/Maths/Vector/vector3.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/light.sld.hpp"

namespace Solid SLDNamespace()
{
	class Light;
	struct LightData
	{
		void* id = nullptr;
		Light* light;
	};

    class SOLID_API SLDClass() Light : public Components
    {
    private:
        static std::vector<LightData> lightList;
        void* id = nullptr;
    public:

        enum SLDEnum() ELightType
        {
            DIRECTIONAL = 0,
            POINT,
            SPOT
        };


        SLDField()
        ELightType type = ELightType::DIRECTIONAL;
	    SLDField()
        Vec3 color = {1,1,1};
	    SLDField()
	    float  intensity = 1;
        SLDField()
        Vec3 dir;

        Light() ;
        Light(const Light& _light);
        Light& operator=(const Light& _light);
        ~Light() override ;

        void Init() override;
        void Release() override;

        static std::vector<LightData> GetLightList();

        Light_GENERATED
    };
} //!namespace

File_GENERATED