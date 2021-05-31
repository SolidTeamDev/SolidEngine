#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Boss.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Boss : public Script
	{
	public:
		
	private:
		
	public:
		Boss();
		~Boss();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		Boss_GENERATED
	};
	
	
	
}

File_GENERATED
