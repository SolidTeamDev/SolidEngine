#include "Core/engine.hpp"
#include "Resources/graphicalResource.hpp"
#include "ECS/Components/particleEffect.hpp"
#include "imgui.h"

#include <iostream>
#include "Time/time.hpp"
#include <string>

using namespace Solid;


void ParticleEffect::Init()
{
	Components::Init();
	const size_t NUM_PARTICLES = 2000;//numParticles == 0 ? 100000 : numParticles;
	system = std::make_shared<ParticleSystem>(NUM_PARTICLES);
	InitializeRenderer();
	ParticleTex = Engine::GetInstance()->graphicsResourceMgr.GetTexture("particle2.png");
	emitter = std::make_shared<ParticleEmitter>();
	emitter->emitRate = (float)NUM_PARTICLES * 0.25f;

	// pos:
	boxPosGen = std::make_shared<BoxPosGen>();
	boxPosGen->pos = Vec4( 0.0f, 0.0f, 0.0f, 0.0f );
	boxPosGen->maxStartPosOffset = Vec4( 0.0f, 0.0f, 0.0f, 0.0f);
	emitter->AddGenerator(boxPosGen);

	colGen = std::make_shared<BasicColorGen>();
	colGen->minStartCol = Vec4( 0.7, 0.7, 0.7, 1.0 );
	colGen->maxStartCol = Vec4( 1.0, 1.0, 1.0, 1.0 );
	colGen->minEndCol   = Vec4( 0.5, 0.0, 0.6, 0.0 );
	colGen->maxEndCol   = Vec4( 0.7, 0.5, 1.0, 0.0 );
	emitter->AddGenerator(colGen);

	velGen = std::make_shared<BasicVelGen>();
	velGen->minStartVel = Vec4( -0.2f, -0.2f, -0.2f, 0.0f );
	velGen->maxStartVel = Vec4( 0.2f, 0.2f, 0.2f, 0.0f );
	emitter->AddGenerator(velGen);

	timeGen = std::make_shared<BasicTimeGen>();
	timeGen->minTime = 5.0f;
	timeGen->maxTime = 7.0f;
	emitter->AddGenerator(timeGen);
	system->AddEmitter(emitter);

	timeUpdater = std::make_shared<BasicTimeUpdater>();
	system->AddUpdater(timeUpdater);

	colorUpdater = std::make_shared<BasicColorUpdater>();
	system->AddUpdater(colorUpdater);

	eulerUpdater = std::make_shared<EulerUpdater>();
	eulerUpdater->globalAcceleration = Vec4(0.0, 0.0, 0.0, 0.0) ;
	system->AddUpdater(eulerUpdater);

}

bool ParticleEffect::InitializeRenderer()
{
	renderer = std::make_shared<GLParticleRenderer>();
	renderer->Generate(system.get(), false);
	return true;
}

void ParticleEffect::Release()
{
	Components::Release();
}

int ParticleEffect::NumAliveParticles()
{
	return system->NumAliveParticles();
}

int ParticleEffect::NumAllParticles()
{
	return system->NumAllParticles();
}

void ParticleEffect::Update(const Transform& trsf)
{
	CpuUpdate();
	GpuUpdate();
	Render();
}

void ParticleEffect::CpuUpdate()
{
	system->Update(Time::DeltaTime());
}

void ParticleEffect::GpuUpdate()
{
	renderer->Update();
}

void ParticleEffect::Render()
{
	if(ParticleTex == nullptr)
		return;
	GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
	glActiveTexture(GL_TEXTURE0);
	GLuint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
	GLuint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
	GLuint last_sampler; { glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler); }
	GLuint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
	GLuint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
	GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
	GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
	GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
	GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
	GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	ParticleTex->BindTexture(0);
	renderer->Render();

	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindSampler(0, last_sampler);
	glActiveTexture(last_active_texture);
	glBindVertexArray(last_vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	glLineWidth(1);

}

void ParticleEffect::SetTex(const std::shared_ptr<ITexture> _tex)
{
	ParticleTex = _tex;
}

std::shared_ptr<ITexture> ParticleEffect::GetTex()
{
	return ParticleTex;
}

void ParticleEffect::UpdateSystem()
{
	system->updaters.clear();
	if (attractorUpdater != nullptr)
		system->AddUpdater(attractorUpdater);
	if (velColUpdater != nullptr)
		system->AddUpdater(velColUpdater);
	if (posColUpdater != nullptr)
		system->AddUpdater(posColUpdater);
	if (colorUpdater != nullptr)
		system->AddUpdater(colorUpdater);
	if (eulerUpdater != nullptr)
		system->AddUpdater(eulerUpdater);
	if (floorUpdater != nullptr)
		system->AddUpdater(floorUpdater);
	if (timeUpdater != nullptr)
		system->AddUpdater(timeUpdater);
	std::cout << "yy\n";
}

void ParticleEffect::UpdateEmitter()
{
	if (emitter == nullptr)
		emitter = std::make_shared<ParticleEmitter>();

	emitter->generators.clear();
	if (velFromPosGen != nullptr)
		emitter->AddGenerator(velFromPosGen);
	if (colGen != nullptr)
		emitter->AddGenerator(colGen);
	if (sphereVelGen != nullptr)
		emitter->AddGenerator(sphereVelGen);
	if (circlePosGen != nullptr)
		emitter->AddGenerator(circlePosGen);
	if (timeGen != nullptr)
		emitter->AddGenerator(timeGen);
	if (velGen != nullptr)
		emitter->AddGenerator(velGen);
	if (boxPosGen != nullptr)
		emitter->AddGenerator(boxPosGen);

	system->AddEmitter(emitter);
}

void ParticleEffect::EditUpdaters()
{
	bool upt = false;

	if (UI::TreeNode("Attractor Updater"))
	{
		bool created = true;
		if (attractorUpdater == nullptr)
			created = false;
		if (UI::Checkbox("Enable##AttractorUpdaterCreated", &created))
		{
			if (created)
				attractorUpdater = std::make_shared<AttractorUpdater>();
			else
				attractorUpdater = nullptr;

			upt = true;
		}
		if (created)
			attractorUpdater->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Euler Updater"))
	{
		bool created = true;
		if (eulerUpdater == nullptr)
			created = false;
		if (UI::Checkbox("Enable##EulerUpdaterCreated", &created))
		{
			if (created)
				eulerUpdater = std::make_shared<EulerUpdater>();
			else
				eulerUpdater = nullptr;

			upt = true;
		}
		if (created)
			eulerUpdater->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Floor Updater"))
	{
		bool created = true;
		if (floorUpdater == nullptr)
			created = false;
		if (UI::Checkbox("Enable##FloorUpdaterCreated", &created))
		{
			if (created)
				floorUpdater = std::make_shared<FloorUpdater>();
			else
				floorUpdater = nullptr;

			upt = true;
		}
		if (created)
			floorUpdater->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Pos Color Updater"))
	{
		bool created = true;
		if (posColUpdater == nullptr)
			created = false;
		if (UI::Checkbox("Enable##PosColorUpdaterCreated", &created))
		{
			if (created)
				posColUpdater = std::make_shared<PosColorUpdater>();
			else
				posColUpdater = nullptr;

			upt = true;
		}
		if (created)
			posColUpdater->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Vel Color Updater"))
	{
		bool created = true;
		if (velColUpdater == nullptr)
			created = false;
		if (UI::Checkbox("Enable##VelColUpdaterCreated", &created))
		{
			if (created)
				velColUpdater = std::make_shared<VelColorUpdater>();
			else
				velColUpdater = nullptr;

			upt = true;
		}
		if (created)
			velColUpdater->ShowUI(upt);
        UI::TreePop();
    }
	if (upt)
		UpdateSystem();
}

void ParticleEffect::EditGenerators()
{
	bool upt = false;

	if (UI::TreeNode("Vel From Pos Generator"))
	{
		bool created = true;
		if (velFromPosGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##VelFromPosGenCreated", &created))
		{
			if (created)
				velFromPosGen = std::make_shared<VelFromPosGen>();
			else
				velFromPosGen = nullptr;

			upt = true;
		}
		if (created)
			velFromPosGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Color Generator"))
	{
		bool created = true;
		if (colGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##ColorGeneratorCreated", &created))
		{
			if (created)
				colGen = std::make_shared<BasicColorGen>();
			else
				colGen = nullptr;

			upt = true;
		}
		if (created)
			colGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Sphere Velocity Generator"))
	{
		bool created = true;
		if (sphereVelGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##SphereVelGenCreated", &created))
		{
			if (created)
				sphereVelGen = std::make_shared<SphereVelGen>();
			else
				sphereVelGen = nullptr;

			upt = true;
		}
		if (created)
			sphereVelGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Circle Pos Generator"))
	{
		bool created = true;
		if (circlePosGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##CirclePosGenCreated", &created))
		{
			if (created)
				circlePosGen = std::make_shared<CirclePosGen>();
			else
				circlePosGen = nullptr;

			upt = true;
		}
		if (created)
			circlePosGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Time Generator"))
	{
		bool created = true;
		if (timeGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##TimeGeneratorCreated", &created))
		{
			if (created)
				timeGen = std::make_shared<BasicTimeGen>();
			else
				timeGen = nullptr;

			upt = true;
		}
		if (created)
			timeGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Velocity Generator"))
	{
		bool created = true;
		if (velGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##VelGeneratorCreated", &created))
		{
			if (created)
				velGen = std::make_shared<BasicVelGen>();
			else
				velGen = nullptr;

			upt = true;
		}
		if (created)
			velGen->ShowUI(upt);
        UI::TreePop();
    }
	if (UI::TreeNode("Box Pos Generator"))
	{
		bool created = true;
		if (boxPosGen == nullptr)
			created = false;
		if (UI::Checkbox("Enable##BoxPosGeneratorCreated", &created))
		{
			if (created)
				boxPosGen = std::make_shared<BoxPosGen>();
			else
				boxPosGen = nullptr;

			upt = true;
		}
		if (created)
			boxPosGen->ShowUI(upt);
		UI::TreePop();
	}
	if (upt)
		UpdateEmitter();
}