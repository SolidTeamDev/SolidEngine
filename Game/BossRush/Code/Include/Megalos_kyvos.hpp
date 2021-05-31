#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Megalos_kyvos.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Megalos_kyvos : public Script
	{
	public:
		
	private:
		
	public:
		Megalos_kyvos();
		~Megalos_kyvos();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		Megalos_kyvos_GENERATED
	};
	
	
	
}

File_GENERATED
