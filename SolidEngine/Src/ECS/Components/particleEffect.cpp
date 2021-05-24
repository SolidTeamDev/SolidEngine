#include "ECS/Components/particleEffect.hpp"

#include <string>
#include "Rendering/FX/Particles/fountainEffect.hpp"

using namespace Solid;

void ParticleEffect::Init()
{
	Components::Init();
}

void ParticleEffect::Release()
{
	Components::Release();
}

int ParticleEffect::NumAliveParticles()
{
	return system->NumAliveParticles();
}

int ParticleEffect::NumAllParticles()
{
	return system->NumAllParticles();
}
