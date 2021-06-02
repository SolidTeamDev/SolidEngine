#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/MikriSfaira.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() MikriSfaira : public Script
	{
	public:
		
	private:
		
	public:
		MikriSfaira();
		~MikriSfaira();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		MikriSfaira_GENERATED
	};
	
	
	
}

File_GENERATED
