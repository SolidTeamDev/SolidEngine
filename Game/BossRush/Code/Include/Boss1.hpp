#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Boss1.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Boss1 : public Script
	{
	public:
    SLDField()
    float Pv = 20;

	private:

	public:
		Boss1();
		~Boss1();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		Boss1_GENERATED
	};
	
	
	
}

File_GENERATED
