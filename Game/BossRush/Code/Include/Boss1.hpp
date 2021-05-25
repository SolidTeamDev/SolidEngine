#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "Core/Maths/solidMaths.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Boss1.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Boss1 : public Script
	{
	public:
    SLDField()
    float Pv = 20;
    SLDField()
    unsigned int PlayerEntityId;
	private:
        GameObject* Player = nullptr;
        Vec3 target;

	public:
		Boss1();
		~Boss1();

		void Init()   override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		void UpdateTarget();
		Boss1_GENERATED
	};
	
	
	
}

File_GENERATED
