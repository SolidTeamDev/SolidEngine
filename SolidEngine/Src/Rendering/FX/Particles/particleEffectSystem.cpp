#include "Rendering/FX/Particles/particleEffectSystem.hpp"

using namespace Solid;

void ParticleEffectSystem::AddEffect(const std::shared_ptr<IEffect>& eff)
{
	effects.push_back(eff);
	eff->ID = effects.size()-1;
}

void ParticleEffectSystem::Init()
{
	effects.clear();
}

void ParticleEffectSystem::Clear()
{
	for (auto& effect : effects)
		effect->Clean();
	effects.clear();
}

void ParticleEffectSystem::Update(double dt)
{
	for (auto & effect : effects)
	{
		effect->Update(dt);
		effect->CpuUpdate(dt);
		effect->GpuUpdate(dt);
	}
}

void ParticleEffectSystem::Erase(size_t ID)
{
	if (effects.size() <= ID)
		return;
	effects[ID]->Clean();
	for (size_t idx = ID+1; idx < effects.size(); idx++)
		effects[idx]->ID--;
	effects.erase(effects.begin()+ID-1);
}

