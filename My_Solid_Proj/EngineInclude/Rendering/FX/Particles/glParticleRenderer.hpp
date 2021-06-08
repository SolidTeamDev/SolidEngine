#pragma once

#include "particleSystem.hpp"

namespace Solid
{

		class SOLID_API GLParticleRenderer
		{
		protected:
			ParticleSystem *system{nullptr};

			unsigned int bufPos{0};
			unsigned int bufCol{0};
			unsigned int vao{0};
		public:
			GLParticleRenderer()
			{}

			~GLParticleRenderer()
			{
				Destroy();
			}

			void Generate(ParticleSystem *sys, bool useQuads);

			void Destroy();

			void Update();

			void Render();
		};

}
