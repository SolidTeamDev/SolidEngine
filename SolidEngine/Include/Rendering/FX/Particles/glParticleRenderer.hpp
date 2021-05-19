#pragma once

#include "particleRenderer.hpp"

namespace Solid
{
	class GLParticleRenderer : public IParticleRenderer
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

		void Generate(ParticleSystem *sys, bool useQuads) override;

		void Destroy() override;

		void Update() override;

		void Render() override;
	};
}
