#pragma once

#include "particleEffect.hpp"
#include <vector>

namespace Solid
{
	class SOLID_API ParticleEffectSystem
	{
	public:
		ParticleEffectSystem() = default;

		~ParticleEffectSystem() = default;

		void Init();
		void Update(double dt);
		void Clear();
		void AddEffect(const std::shared_ptr<IEffect>& eff);
		void Erase(size_t ID);

	protected:
		std::vector<std::shared_ptr<IEffect>> effects;
	};
}
