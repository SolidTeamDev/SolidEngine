#include "ECS/System/particleEffectSystem.hpp"
#include "glad/glad.h"
using namespace Solid;
using namespace Solid::Particles;

ParticleEffectSystem::ParticleEffectSystem(ECSManager& _ecsManager)
	: System(_ecsManager)
{
	ShaderRaw = new ShaderResource();
	ShaderRaw->VertexSource = "";
	ShaderRaw->FragSource = "";


}


void ParticleEffectSystem::Update()
{
	program->BindShader();
	for (auto& entity : entities)
	{
		const Transform& trsf = ecsManager.GetComponent<Transform>(entity);
		ParticleEffect& particleEffect = ecsManager.GetComponent<ParticleEffect>(entity);
		particleEffect.Update(trsf);
	}
	program->UnbindShader();
}

void ParticleEffectSystem::InitShaderForGL()
{
	program = std::make_shared<GL::Shader>(ShaderRaw);
}


