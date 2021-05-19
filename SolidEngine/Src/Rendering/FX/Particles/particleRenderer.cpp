#include "Rendering/FX/Particles/particleRenderer.hpp"
#include "Rendering/FX/Particles/glParticleRenderer.hpp"

#include <string>

using namespace Solid;

std::shared_ptr<IParticleRenderer> RendererFactory::Create(const char* name)
{
	std::string renderer{ name };

	if (renderer == "gl")
		return std::make_shared<GLParticleRenderer>();

	return nullptr;
}
