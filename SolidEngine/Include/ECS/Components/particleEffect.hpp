#pragma once

#include <memory>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/Transform.hpp"

#include "Rendering/FX/Particles/particleData.hpp"
#include "Rendering/FX/Particles/particleGenerator.hpp"
#include "Rendering/FX/Particles/particleUpdater.hpp"
#include "Rendering/FX/Particles/particleSystem.hpp"

#include "Rendering/FX/Particles/glParticleRenderer.hpp"

namespace Solid
{
	namespace Particles
	{
		class SOLID_API SLDClass() ParticleEffect : public Components
		{
			SLDField()
			size_t numParticles;

			std::shared_ptr<ParticleSystem> system = nullptr;

			std::shared_ptr<VelFromPosGen> velFromPosGen = nullptr;
			std::shared_ptr<BasicColorGen> colGen = nullptr;
			std::shared_ptr<CirclePosGen> circlePosGen = nullptr;
			std::shared_ptr<SphereVelGen> sphereVelGen = nullptr;
			std::shared_ptr<BasicTimeGen> timeGen = nullptr;
			std::shared_ptr<BasicVelGen> velGen = nullptr;
			std::shared_ptr<BoxPosGen> boxPosGen = nullptr;

			std::shared_ptr<AttractorUpdater> attractorUpdater = nullptr;
			std::shared_ptr<VelColorUpdater> velColUpdater = nullptr;
			std::shared_ptr<PosColorUpdater> posColUpdater = nullptr;
			std::shared_ptr<BasicColorUpdater> colorUpdater = nullptr;
			std::shared_ptr<EulerUpdater> eulerUpdater = nullptr;
			std::shared_ptr<FloorUpdater> floorUpdater = nullptr;
			std::shared_ptr<BasicTimeUpdater> timeUpdater = nullptr;

			std::shared_ptr<GLParticleRenderer> renderer = nullptr;

		public:
			uint defaultParticleNb = 0;

			ParticleEffect() = default;
			~ParticleEffect() override = default;


			void Init() override;
			void Release() override;

			// creates the effect with desired num of particles, (0 means default for the effect)
			bool Initialize(size_t numParticles);
			bool InitializeRenderer();

			void Update(const Transform& trsf);
			void CpuUpdate();
			void GpuUpdate();
			void Render();

			int NumAllParticles();
			int NumAliveParticles();
		};
	}
}
