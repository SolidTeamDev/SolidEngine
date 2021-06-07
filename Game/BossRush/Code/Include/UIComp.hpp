#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/UIComp.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() UIComp : public Script
	{
	public:
		
	private:
		
	public:
		UIComp();
		~UIComp();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		UIComp_GENERATED
	};
	
	
	
}

File_GENERATED
