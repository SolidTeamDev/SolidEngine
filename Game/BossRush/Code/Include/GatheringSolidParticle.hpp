#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/particleEffect.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/GatheringSolidParticle.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() GatheringSolidParticle : public Script
	{
	private:
		ParticleEffect* effect = nullptr;

		SLDField()
		float gatherTime = 0.f;

		float currGather = 0.f;

		SLDField()
		bool gathering = false;
	public:
		GatheringSolidParticle();
		~GatheringSolidParticle();

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		void Destroy() override;
		GatheringSolidParticle_GENERATED
	};
	
	
	
}

File_GENERATED
