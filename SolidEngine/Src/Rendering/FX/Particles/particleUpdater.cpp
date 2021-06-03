#include "Rendering/FX/Particles/ParticleUpdater.hpp"
#include "imgui.h"

using namespace Solid;

float Mix(float x, float y, float a)
{
	return x * (1.0f - a) + y * a;
}

float Dot4(Vec4 a, Vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


void EulerUpdater::Update(float dt, ParticleData* p)
{
	const Vec4 globalA{ dt * globalAcceleration.x, dt * globalAcceleration.y, dt * globalAcceleration.z, 0.0 };
	const float localDT = (float)dt;

	Vec4* __restrict acc = p->acc.get();
	Vec4* __restrict vel = p->vel.get();
	Vec4* __restrict pos = p->pos.get();

	const unsigned int endId = p->countAlive;
	for (size_t i = 0; i < endId; ++i)
	{
		acc[i].x += globalA.x;
		acc[i].y += globalA.y;
		acc[i].z += globalA.z;
		acc[i].w += globalA.w;
	}
	for (size_t i = 0; i < endId; ++i)
	{
		vel[i].x += localDT * acc[i].x;
		vel[i].y += localDT * acc[i].y;
		vel[i].z += localDT * acc[i].z;
		vel[i].w += localDT * acc[i].w;
	}
	for (size_t i = 0; i < endId; ++i)
	{
		pos[i].x += localDT * vel[i].x;
		pos[i].y += localDT * vel[i].y;
		pos[i].z += localDT * vel[i].z;
		pos[i].w += localDT * vel[i].w;
	}
}



void FloorUpdater::Update(float dt, ParticleData* p)
{
	const float localDT = (float)dt;

	Vec4* __restrict acc = p->acc.get();
	Vec4* __restrict vel = p->vel.get();
	Vec4* __restrict pos = p->pos.get();

	const size_t endId = p->countAlive;
	for (size_t i = 0; i < endId; ++i)
	{
		if (pos[i].y < floorY)
		{
			Vec4 force = p->acc[i];
			float normalFactor = Dot4(force, Vec4(0.0f, 1.0f, 0.0f, 0.0f));
			if (normalFactor < 0.0f)
			{
				force.y -= normalFactor;

			}
			float velFactor = Dot4(vel[i], Vec4(0.0f, 1.0f, 0.0f, 0.0f));
			//if (velFactor < 0.0)
			vel[i].y -= (1.0f + bounceFactor) * velFactor;

			acc[i] = force;
		}
	}

}

void AttractorUpdater::Update(float dt, ParticleData* p)
{
	const float localDT = dt;

	Vec4* __restrict acc = p->acc.get();
	Vec4* __restrict vel = p->vel.get();
	Vec4* __restrict pos = p->pos.get();

	const size_t endId = p->countAlive;
	const size_t countAttractors = attractors.size();
	Vec4 off;
	float dist;
	size_t a;
	for (size_t i = 0; i < endId; ++i)
	{
		for (a = 0; a < countAttractors; ++a)
		{
			off.x = attractors[a].x - pos[i].x;
			off.y = attractors[a].y - pos[i].y;
			off.z = attractors[a].z - pos[i].z;
			off.w = attractors[a].w - pos[i].w;
			dist = Dot4(off, off);

			//if (fabs(dist) > 0.00001)
			dist = attractors[a].w / dist;

			acc[i].x += off.x * dist;
			acc[i].y += off.y * dist;
			acc[i].z += off.z * dist;
			acc[i].w += off.w * dist;
		}
	}
}

void BasicColorUpdater::Update(float dt, ParticleData* p)
{
	Vec4* __restrict col = p->col.get();
	Vec4* __restrict startCol = p->startCol.get();
	Vec4* __restrict endCol = p->endCol.get();
	Vec4* __restrict t = p->time.get();

	const size_t endId = p->countAlive;
	for (size_t i = 0; i < endId; ++i)
	{
		col[i].x = Mix(startCol[i].x, endCol[i].x, t[i].z);
		col[i].y = Mix(startCol[i].y, endCol[i].y, t[i].z);
		col[i].z = Mix(startCol[i].z, endCol[i].z, t[i].z);
		col[i].w = Mix(startCol[i].w, endCol[i].w, t[i].z);
	}
}

void PosColorUpdater::Update(float dt, ParticleData* p)
{
	Vec4* __restrict col      = p->col.get();
	Vec4* __restrict startCol = p->startCol.get();
	Vec4* __restrict endCol   = p->endCol.get();
	Vec4* __restrict t        = p->time.get();
	Vec4* __restrict pos      = p->pos.get();

	const int endId = (int)p->countAlive;
	float scaler, scaleg, scaleb;
	float diffr = maxPos.x - minPos.x;
	float diffg = maxPos.y - minPos.y;
	float diffb = maxPos.z - minPos.z;

	for (int i = 0; i < endId; ++i)
	{
		scaler = (pos[i].x - minPos.x) / diffr;
		scaleg = (pos[i].y - minPos.y) / diffg;
		scaleb = (pos[i].z - minPos.z) / diffb;
		col[i].x = scaler;
		col[i].y = scaleg;
		col[i].z = scaleb;
		col[i].w = Mix(startCol[i].w, endCol[i].w, t[i].z);
	}
}

void VelColorUpdater::Update(float dt, ParticleData* p)
{
	Vec4* __restrict col      = p->col.get();
	Vec4* __restrict startCol = p->startCol.get();
	Vec4* __restrict endCol   = p->endCol.get();
	Vec4* __restrict t        = p->time.get();
	Vec4* __restrict vel      = p->vel.get();

	const size_t endId = p->countAlive;
	float scaler, scaleg, scaleb;
	float diffr = maxVel.x - minVel.x;
	float diffg = maxVel.y - minVel.y;
	float diffb = maxVel.z - minVel.z;
	for (size_t i = 0; i < endId; ++i)
	{
		scaler = (vel[i].x - minVel.x) / diffr;
		scaleg = (vel[i].y - minVel.y) / diffg;
		scaleb = (vel[i].z - minVel.z) / diffb;
		col[i].x = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
		col[i].y = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
		col[i].z = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
		col[i].w = Mix(startCol[i].w, endCol[i].w, t[i].z);
	}
}

void BasicTimeUpdater::Update(float dt, ParticleData* p)
{
	unsigned int endId = p->countAlive;
	const float localDT = (float)dt;

	Vec4* __restrict t = p->time.get();

	if (endId == 0) return;

	for (size_t i = 0; i < endId; ++i)
	{
		t[i].x -= localDT;
		// interpolation: from 0 (start of life) till 1 (end of life)
		t[i].z = (float)1.0 - (t[i].x * t[i].w); // .w is 1.0/max life time		

		if (t[i].x < (float)0.0)
		{
			p->Kill(i);
			endId = p->countAlive < p->count ? p->countAlive : p->count;
		}
	}
}

void KillerZoneUpdater::Update(float dt, ParticleData *p)
{
	Vec4* __restrict _pos      = p->pos.get();

	unsigned int endId = p->countAlive;
	for (size_t i = 0; i < endId; ++i)
	{
		if     (_pos[i].x <= pos.x + offset.x && _pos[i].x >= pos.x - offset.x &&
				_pos[i].y <= pos.y + offset.y && _pos[i].y >= pos.y - offset.y &&
				_pos[i].z <= pos.z + offset.z && _pos[i].z >= pos.z - offset.z)
			p->Kill(i);
	}
}

void KillerZoneUpdater::ShowUI(bool &upt)
{
	ImVec4 _pos(pos.x, pos.y, pos.z, 0.f);
	if (UI::DragFloat4("Pos##poscol", &_pos.x))
	{
		pos = {_pos.x, _pos.y, _pos.z};
		upt = true;
	}
	ImVec4 off(offset.x, offset.y, offset.z, 0.f);
	if (UI::DragFloat4("Offset##poscol", &off.x))
	{
		offset = {off.x, off.y, off.z};
		upt = true;
	}
}

void AttractorUpdater::ShowUI(bool& upt)
{
	if (UI::Button("Create Attractor"))
	{
		Vec4 newVec(0.f, 0.f, 0.f, 0.f);
		attractors.push_back(newVec);
		upt = true;
	}
	for (size_t it = 0; it < attractors.size(); it++)
	{
		ImVec4 _vec(attractors[it].x, attractors[it].y, attractors[it].z, attractors[it].w);
		if (UI::DragFloat4(("Pos##" + std::to_string(it)).c_str(), &_vec.x))
		{
			attractors[it] = {_vec.x, _vec.y, _vec.z, _vec.w};
			upt = true;
		}
	}
}

void PosColorUpdater::ShowUI(bool &upt)
{
	ImVec4 _min(minPos.x, minPos.y, minPos.z, minPos.w);
	if (UI::DragFloat4("Min Pos##poscol", &_min.x))
	{
		minPos = {_min.x, _min.y, _min.z, _min.w};
		upt = true;
	}
	ImVec4 _max(maxPos.x, maxPos.y, maxPos.z, maxPos.w);
	if (UI::DragFloat4("Max Pos##poscol", &_max.x))
	{
		maxPos = {_max.x, _max.y, _max.z, _max.w};
		upt = true;
	}
}

void VelColorUpdater::ShowUI(bool &upt)
{
	ImVec4 _min(minVel.x, minVel.y, minVel.z, minVel.w);
	if (UI::DragFloat4("Min Vel##velcol", &_min.x))
	{
		minVel = {_min.x, _min.y, _min.z, _min.w};
		upt = true;
	}
	ImVec4 _max(maxVel.x, maxVel.y, maxVel.z, maxVel.w);
	if (UI::DragFloat4("Max Vel##velcol", &_max.x))
	{
		maxVel = {_max.x, _max.y, _max.z, _max.w};
		upt = true;
	}
}

void FloorUpdater::ShowUI(bool &upt)
{
	if (UI::DragFloat("Y", &floorY))
		upt = true;
	if (UI::DragFloat("Bounciness##floor", &bounceFactor))
		upt = true;
}

void EulerUpdater::ShowUI(bool &upt)
{
	ImVec4 _acc(globalAcceleration.x, globalAcceleration.y, globalAcceleration.z, globalAcceleration.w);
	if (UI::DragFloat4("Acceleration##euler", &_acc.x))
	{
		globalAcceleration = {_acc.x, _acc.y, _acc.z, _acc.w};
		upt = true;
	}
}


