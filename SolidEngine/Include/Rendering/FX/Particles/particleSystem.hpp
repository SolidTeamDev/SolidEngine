#pragma once

#include <vector>

#include "Rendering/FX/Particles/particleData.hpp"
#include "Rendering/FX/Particles/particleEmitter.hpp"
#include "Rendering/FX/Particles/particleUpdater.hpp"

namespace Solid
{
	namespace Particles
	{
		class SOLID_API ParticleSystem
		{
		public:
			ParticleData particles;
			ParticleData aliveParticles;

			size_t count;

			std::vector<std::shared_ptr<ParticleEmitter>> emitters;
			std::vector<std::shared_ptr<ParticleUpdater>> updaters;

			explicit ParticleSystem(size_t maxCount);

			virtual ~ParticleSystem()
			{}

			//ParticleSystem(const ParticleSystem &);

			//ParticleSystem &operator=(const ParticleSystem &);

			virtual void Update(double dt);

			virtual void Reset();

			virtual size_t NumAllParticles() const
			{ return particles.count; }

			virtual size_t NumAliveParticles() const
			{ return particles.countAlive; }

			void AddEmitter(std::shared_ptr<ParticleEmitter> em)
			{ emitters.push_back(em); }

			void AddUpdater(std::shared_ptr<ParticleUpdater> up)
			{ updaters.push_back(up); }

			ParticleData *FinalData()
			{ return &particles; }

			void SetCount(unsigned int _count)
			{
				count = _count;
				particles.Generate(count);
				aliveParticles.Generate(count);

				for (size_t i = 0; i < count; ++i)
					particles.alive[i] = false;
			}

			size_t GetCount()
			{
				return count;
			}

			void SetEmitRate(float _rate)
			{
				if (_rate < 0.0f)
					_rate = 0.0f;
				emitters[0]->emitRate = _rate;
			}

			float GetEmitRate()
			{
				return emitters[0]->emitRate;
			}

			static size_t ComputeMemoryUsage(const ParticleSystem &p);


		};
	}
}

