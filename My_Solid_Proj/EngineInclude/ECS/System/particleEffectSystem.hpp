#pragma once
#include "Resources/graphicalResource.hpp"
#include "ECS/Components/particleEffect.hpp"
#include "ECS/system.hpp"

#include <vector>

namespace Solid
{

		class SOLID_API ParticleEffectSystem : public System
		{
		public:

			ShaderResource* ShaderRaw = nullptr;
			std::shared_ptr<GL::Shader> program = nullptr;

			ParticleEffectSystem(ECSManager& _ecsManager);
			void InitShaderForGL();
			void Update(Camera& _cam);

		};

}
