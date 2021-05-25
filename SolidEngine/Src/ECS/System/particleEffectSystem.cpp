#include "ECS/System/particleEffectSystem.hpp"
#include "glad/glad.h"
using namespace Solid;
using namespace Solid::Particles;

ParticleEffectSystem::ParticleEffectSystem(ECSManager& _ecsManager)
	: System(_ecsManager)
{
	ShaderRaw = new ShaderResource();
	ShaderRaw->VertexSource =
			                  "#version 450\n"
                              "uniform mat4 proj;\n"
	                          "        uniform mat4 view;\n"
	                          "        uniform mat4 model;\n"
	                          "        \n"
	                          "\n"
	                          "        layout(location = 0) in vec4 vVertex;\n"
	                          "        layout(location = 1) in vec4 vColor;\n"
	                          "\n"
	                          "        out vec4 outColor;\n"
	                          "\n"
	                          "        void main() \n"
	                          "        {\n"
						      "            mat4 pvm = proj * view * model;"
	                          "            vec4 eyePos = view * vVertex;\n"
	                          "            gl_Position = pvm * vVertex;\n"
	                          "\n"
	                          "\t        outColor = vColor;\n"
	                          "\n"
	                          "            float dist = length(eyePos.xyz);\n"
	                          "\t        float att = inversesqrt(0.1*dist);\n"
	                          "\t        gl_PointSize = 3.f * att;\n"
	                          "        }\n";
	ShaderRaw->FragSource = "#version 450\n"
	                        "uniform sampler2D tex;\n"
	                        "\n"
	                        "        in vec4 outColor;\n"
	                        "\n"
	                        "        out vec4 vFragColor;\n"
	                        "\n"
	                        "        void main() \n"
	                        "        {\n"
	                        "\t        vFragColor = texture(tex, gl_PointCoord) * outColor;\n"
	                        "           // vFragColor = outColor;\n"
	                        "        }\n";


}


void ParticleEffectSystem::Update(Camera& _cam)
{
	program->BindShader();

	for (auto& entity : entities)
	{
		Transform& trsf = ecsManager.GetComponent<Transform>(entity);
		ParticleEffect& particleEffect = ecsManager.GetComponent<ParticleEffect>(entity);
		program->SetMVP(trsf, _cam);
		particleEffect.Update(trsf);
	}
	program->UnbindShader();
}

void ParticleEffectSystem::InitShaderForGL()
{
	program = std::make_shared<GL::Shader>(ShaderRaw);
}


