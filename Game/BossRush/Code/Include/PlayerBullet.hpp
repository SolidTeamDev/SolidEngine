#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/PlayerBullet.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() PlayerBullet : public Script
	{
	public:
		
	private:
		
	public:
		PlayerBullet();
		~PlayerBullet();

        void Destroy() override;

        PlayerBullet_GENERATED
	};
	
	
	
}

File_GENERATED
