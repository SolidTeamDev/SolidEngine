#pragma once

#include <memory>

namespace Solid
{
	class ParticleSystem;

	class SOLID_API IParticleRenderer
	{
	public:
		IParticleRenderer()
		{}

		virtual ~IParticleRenderer()
		{}

		virtual void Generate(ParticleSystem *sys, bool useQuads) = 0;

		virtual void Destroy() = 0;

		virtual void Update() = 0;

		virtual void Render() = 0;
	};

	class SOLID_API RendererFactory
	{
	public:
		static std::shared_ptr<IParticleRenderer> Create(const char *name);
	};
}

