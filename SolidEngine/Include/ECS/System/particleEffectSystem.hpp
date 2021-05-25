#pragma once

#include "ECS/Components/particleEffect.hpp"
#include "ECS/system.hpp"

#include <vector>

namespace Solid
{
	namespace Particles
	{
		class SOLID_API ParticleEffectSystem : public System
		{
		public:
			ParticleEffectSystem(ECSManager &_ecsManager) : System(_ecsManager)
			{};

			void Update();

		};
	}
}
