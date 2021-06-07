#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/particleEffect.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "ECS/Components/rigidBody.hpp"
#include "EngineGenerated/followingObj.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() followingObj : public Script
	{
	public:
        GameObject* following = nullptr;
        ParticleEffect* effect= nullptr;
        RigidBody* rb = nullptr;
        Vec3 dir = {0.f,0.f,0.f};

	private:
		
	public:
		followingObj();
		~followingObj();

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		followingObj_GENERATED
	};
	
	
	
}

File_GENERATED
