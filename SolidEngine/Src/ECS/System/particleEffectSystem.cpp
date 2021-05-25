#include "ECS/System/particleEffectSystem.hpp"

using namespace Solid;
using namespace Solid::Particles;
void ParticleEffectSystem::Update()
{
	for (auto& entity : entities)
	{
		const Transform& trsf = ecsManager.GetComponent<Transform>(entity);
		ParticleEffect& particleEffect = ecsManager.GetComponent<ParticleEffect>(entity);
		particleEffect.Update(trsf);
	}
}


