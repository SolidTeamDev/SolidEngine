#include "ECS/Components/particleEffect.hpp"
#include "Time/time.hpp"

#include <string>

using namespace Solid;
using namespace Solid::Particles;

void ParticleEffect::Init()
{
	Components::Init();
}

bool ParticleEffect::InitializeRenderer()
{
	renderer = std::make_shared<GLParticleRenderer>();

	return true;
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

void ParticleEffect::Update(const Transform& trsf)
{
	CpuUpdate();
	GpuUpdate();
}

void ParticleEffect::CpuUpdate()
{
	system->Update(Time::DeltaTime());
}

void ParticleEffect::GpuUpdate()
{
	renderer->Update();
}

void ParticleEffect::Render()
{
	renderer->Render();
}
