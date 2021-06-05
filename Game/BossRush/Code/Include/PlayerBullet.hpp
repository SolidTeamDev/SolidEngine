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
        SLDField()
        float damage = 0;
        SLDField()
        float lifeTime = 0;
	private:

        float currentTime = 0;
	public:
		PlayerBullet();
		~PlayerBullet();

        void Update() override;

        void OnTriggerEnter(GameObject *_other) override;

        PlayerBullet_GENERATED
	};
	
	
	
}

File_GENERATED


