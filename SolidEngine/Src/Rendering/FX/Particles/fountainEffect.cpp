#include "Rendering/FX/Particles/fountainEffect.hpp"
#include "imgui.h"

#include <cmath>
#include <iostream>
#include "Core/Maths/solidVector.hpp"
#include "Time/time.hpp"

using namespace Solid;

bool FountainEffect::Initialize(size_t numParticles)
{
	//
	// particles
	//
	const size_t numP = numParticles == 0 ? 100000 : numParticles;
	system = std::make_shared<ParticleSystem>(numP);

	//
	// emitter:
	//
	auto particleEmitter = std::make_shared<ParticleEmitter>();
	particleEmitter->emitRate = (float)numP * 0.25f;

	// pos:
	posGenerator = std::make_shared<BoxPosGen>();
	posGenerator->pos = Vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
	posGenerator->maxStartPosOffset = Vec4{ 0.0f, 0.0f, 0.0f, 0.0f};
	particleEmitter->AddGenerator(posGenerator);

	colGenerator = std::make_shared<BasicColorGen>();
	colGenerator->minStartCol = Vec4{ 0.7, 0.7, 0.7, 1.0 };
	colGenerator->maxStartCol = Vec4{ 1.0, 1.0, 1.0, 1.0 };
	colGenerator->minEndCol   = Vec4{ 0.5, 0.0, 0.6, 0.0 };
	colGenerator->maxEndCol   = Vec4{ 0.7, 0.5, 1.0, 0.0 };
	particleEmitter->AddGenerator(colGenerator);

	auto velGenerator = std::make_shared<BasicVelGen>();
	velGenerator->minStartVel = Vec4{ -0.05f, 0.62f, -0.05f, 0.0f };
	velGenerator->maxStartVel = Vec4{ 0.05f, 0.85f, 0.05f, 0.0f };
	particleEmitter->AddGenerator(velGenerator);

	auto timeGenerator = std::make_shared<BasicTimeGen>();
	timeGenerator->minTime = 5.0f;
	timeGenerator->maxTime = 7.0f;
	particleEmitter->AddGenerator(timeGenerator);
	system->AddEmitter(particleEmitter);

	auto timeUpdater = std::make_shared<BasicTimeUpdater>();
	system->AddUpdater(timeUpdater);

	auto colorUpdater = std::make_shared<BasicColorUpdater>();
	system->AddUpdater(colorUpdater);

	eulerUpdater = std::make_shared<EulerUpdater>();
	eulerUpdater->globalAcceleration = Vec4{ 0.0, -15.0, 0.0, 0.0 };
	system->AddUpdater(eulerUpdater);

	floorUpdater = std::make_shared<FloorUpdater>();
	system->AddUpdater(floorUpdater);


	

	return true;
}

bool FountainEffect::InitializeRenderer()
{
	renderer = RendererFactory::Create("gl");
	renderer->Generate(system.get(), false);

	return true;
}

void FountainEffect::Clean()
{
	if (renderer) renderer->Destroy();
}

void FountainEffect::ShowUI()
{


	if (colGenerator != nullptr)
	{
		float minStartCol[4] = { colGenerator->minStartCol.x, colGenerator->minStartCol.y, colGenerator->minStartCol.z, colGenerator->minStartCol.w };
		if (ImGui::ColorEdit4("minStartColor", minStartCol))
			colGenerator->minStartCol = { minStartCol[0], minStartCol[1], minStartCol[2], minStartCol[3] };
	}
	if (colGenerator != nullptr)
	{
		float maxStartCol[4] = { colGenerator->maxStartCol.x, colGenerator->maxStartCol.y, colGenerator->maxStartCol.z, colGenerator->maxStartCol.w };
		if(ImGui::ColorEdit4("maxStartColor", maxStartCol))
			colGenerator->maxStartCol = { maxStartCol[0], maxStartCol[1], maxStartCol[2], maxStartCol[3] };
	}

	if (colGenerator != nullptr)
	{
		float minEndCol[4] = { colGenerator->minEndCol.x, colGenerator->minEndCol.y, colGenerator->minEndCol.z, colGenerator->minEndCol.w };
		if (ImGui::ColorEdit4("minEndColor", minEndCol))
			colGenerator->minEndCol = { minEndCol[0], minEndCol[1], minEndCol[2], minEndCol[3] };
	}
	if (colGenerator != nullptr)
	{
		float maxEndCol[4] = { colGenerator->maxEndCol.x, colGenerator->maxEndCol.y, colGenerator->maxEndCol.z, colGenerator->maxEndCol.w };
		if (ImGui::ColorEdit4("maxEndColor", maxEndCol))
			colGenerator->maxEndCol = { maxEndCol[0], maxEndCol[1], maxEndCol[2], maxEndCol[3] };
	}
}

void FountainEffect::Update(double dt)
{
	posGenerator->pos.x = 0.9f * sin(Time::DeltaTime() * 2.5f);
	posGenerator->pos.z = 0.9f * cos(Time::DeltaTime() * 2.5f);
}

void FountainEffect::CpuUpdate(double dt)
{
	system->Update(dt);
}

void FountainEffect::GpuUpdate(double dt)
{
	renderer->Update();
}

void FountainEffect::Render()
{
	renderer->Render();
}