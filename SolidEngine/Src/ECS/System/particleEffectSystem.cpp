#include "ECS/System/particleEffectSystem.hpp"

using namespace Solid;

void ParticleEffectSystem::Update(double dt, ParticleEffect& eff)
{
	eff.Update(dt);
	eff.CpuUpdate(dt);
	eff.GpuUpdate(dt);
}


