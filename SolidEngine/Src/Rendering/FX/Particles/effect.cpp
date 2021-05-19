#include "Rendering/FX/Particles/effect.hpp"

#include <string>
#include "Rendering/FX/Particles/fountainEffect.hpp"

using namespace Solid;

std::shared_ptr<IEffect> EffectFactory::Create(const char* name)
{
	std::string effect{ name };

	//if (effect == "tunnel")
		//return std::make_shared<TunnelEffect>();
	//else if (effect == "attractors")
		//return std::make_shared<AttractorEffect>();
	if (effect == "fountain")
		return std::make_shared<FountainEffect>();

	return nullptr;
}