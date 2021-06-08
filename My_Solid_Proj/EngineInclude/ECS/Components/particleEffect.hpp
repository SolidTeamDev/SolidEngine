#pragma once

#include <memory>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/Transform.hpp"

#include "Rendering/FX/Particles/particleData.hpp"
#include "Rendering/FX/Particles/particleGenerator.hpp"
#include "Rendering/FX/Particles/particleUpdater.hpp"
#include "Rendering/FX/Particles/particleSystem.hpp"
#include "Resources/graphicalResourceInterface.hpp"
#include "Rendering/FX/Particles/glParticleRenderer.hpp"
#include "EngineGenerated/particleEffect.sld.hpp"

namespace Solid SLDNamespace()
{

		class SOLID_API SLDClass() ParticleEffect : public Components
		{
			SLDField()
			size_t numParticles;


			std::shared_ptr<ParticleSystem> system = nullptr;
			std::shared_ptr<ParticleEmitter> emitter = nullptr;

			std::shared_ptr<ITexture> ParticleTex = nullptr;
			std::shared_ptr<GLParticleRenderer> renderer = nullptr;

		public:

			size_t GetNumParticle()
			{
				return numParticles;
			}
			std::shared_ptr<ParticleSystem> GetSystem()
			{
				return system;
			}
			std::shared_ptr<ParticleEmitter> GetEmitter()
			{
				return emitter;
			}
			std::shared_ptr<ITexture> GetParticleTex()
			{
				return ParticleTex;
			}
			std::shared_ptr<GLParticleRenderer> GetRenderer()
			{
				return renderer;
			}

			SLDField()
			float particlesSize;

			std::shared_ptr<VelFromPosGen> velFromPosGen = nullptr;
			std::shared_ptr<BasicColorGen> colGen        = nullptr;
			std::shared_ptr<SphereVelGen>  sphereVelGen  = nullptr;
			std::shared_ptr<SpherePosGen>  spherePosGen  = nullptr;
			std::shared_ptr<BasicTimeGen>  timeGen       = nullptr;
			std::shared_ptr<BasicVelGen>   velGen        = nullptr;
			std::shared_ptr<BoxPosGen>     boxPosGen     = nullptr;



			std::shared_ptr<KillerZoneUpdater>  killerZoneUpdater= nullptr;
			std::shared_ptr<AttractorUpdater>   attractorUpdater = nullptr;
			std::shared_ptr<VelColorUpdater>    velColUpdater    = nullptr;
			std::shared_ptr<PosColorUpdater>    posColUpdater    = nullptr;
			std::shared_ptr<BasicColorUpdater>  colorUpdater     = nullptr;
			std::shared_ptr<EulerUpdater>       eulerUpdater     = nullptr;
			std::shared_ptr<FloorUpdater>       floorUpdater     = nullptr;
			std::shared_ptr<BasicTimeUpdater>   timeUpdater      = nullptr;

		public:
			uint defaultParticleNb = 0;

			ParticleEffect() = default;
			~ParticleEffect() override = default;


			void Init() override;
			void Release() override;

			bool InitializeRenderer();

			void Update(const Transform& trsf);
			void CpuUpdate();
			void GpuUpdate();
			void Render();


			void SetTex(const std::shared_ptr<ITexture> _tex);
			void SetCount(size_t _count)
			{
				numParticles = _count;
				system->SetCount(_count);
				renderer->Destroy();
				renderer->Generate(system.get(), true);
			}
			size_t GetCount()
			{
				return system->count;
			}
			void SetEmitRate(float _rate)
			{
				system->SetEmitRate(_rate);
			}

			float GetEmitRate()
			{
				return system->GetEmitRate();
			}
			std::shared_ptr<ITexture> GetTex();
			int NumAllParticles();
			int NumAliveParticles();

			void UpdateSystem();
			void UpdateEmitter();

			void EditUpdaters();
			void EditGenerators();
			ParticleEffect_GENERATED
		};

}
File_GENERATED