#pragma once

#include <memory>

namespace Solid
{
	class SOLID_API IEffect
	{
	public:
		static const size_t DEFAULT_PARTICLE_NUM_FLAG = 0;

		size_t ID = 0;
	public:
		IEffect()
		{}

		virtual ~IEffect()
		{}

		// creates the effect with desired num of particles, (0 means default for the effect)
		virtual bool Initialize(size_t numParticles) = 0;

		virtual bool InitializeRenderer() = 0;

		virtual void Reset() = 0;

		virtual void Clean() = 0;

		virtual void ShowUI() = 0;

		virtual void Update(double dt) = 0;

		virtual void CpuUpdate(double dt) = 0;

		virtual void GpuUpdate(double dt) = 0;

		virtual void Render() = 0;

		virtual int NumAllParticles() = 0;

		virtual int NumAliveParticles() = 0;
	};

	class SOLID_API EffectFactory
	{
	public:
		static std::shared_ptr<IEffect> Create(const char *name);
	};
}
