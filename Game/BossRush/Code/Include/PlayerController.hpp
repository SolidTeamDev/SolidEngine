#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/PlayerController.sld.hpp"
namespace Solid SLDNamespace()
{
    class Engine;
	class SOLID_SCRIPT_API SLDClass() PlayerController : public Script
	{
	public:
		
	private:
		Engine* engine = nullptr;
	public:
		PlayerController();
		~PlayerController() override = default;
		
		void Update() override;

		PlayerController_GENERATED
	};
	
	
	
}

File_GENERATED
