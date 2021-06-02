#include "Rendering/FX/Particles/particleGenerator.hpp"
#include <random>
#include "imgui.h"

#define M_PI 3.14159265358979323846

using namespace Solid;

float LinearRand(float min, float max)
{
	return ((rand() % 100) / 100.f) * (max - min) + min;
}


void BoxPosGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	Vec4 posMin{ pos.x - maxStartPosOffset.x, pos.y - maxStartPosOffset.y, pos.z - maxStartPosOffset.z, 1.0 };
	Vec4 posMax{ pos.x + maxStartPosOffset.x, pos.y + maxStartPosOffset.y, pos.z + maxStartPosOffset.z, 1.0 };

	for (size_t i = startId; i < endId; ++i)
	{
		p->pos[i].x = LinearRand(posMin.x, posMax.x);
		p->pos[i].y = LinearRand(posMin.y, posMax.y);
		p->pos[i].z = LinearRand(posMin.z, posMax.z);
		p->pos[i].w = LinearRand(posMin.w, posMax.w);
	}
}

void SpherePosGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		double theta = LinearRand(0.0, M_PI);
		double rand = LinearRand(0, 1);
		double phi = LinearRand(0.0, M_PI * 2.0);

		p->pos[i].x = center.x + radX * sin(theta) * cos (phi);
		p->pos[i].y = center.y + radY * sin(theta) * sin (phi);
		p->pos[i].z = center.z + radZ * cos(theta);
		p->pos[i].w = center.w + 1.0;
	}
}

void BasicColorGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		p->startCol[i].x = LinearRand(minStartCol.x, maxStartCol.x);
		p->startCol[i].y = LinearRand(minStartCol.y, maxStartCol.y);
		p->startCol[i].z = LinearRand(minStartCol.z, maxStartCol.z);
		p->startCol[i].w = LinearRand(minStartCol.w, maxStartCol.w);

		p->endCol[i].x = LinearRand(minEndCol.x, maxEndCol.x);
		p->endCol[i].y = LinearRand(minEndCol.y, maxEndCol.y);
		p->endCol[i].z = LinearRand(minEndCol.z, maxEndCol.z);
		p->endCol[i].w = LinearRand(minEndCol.w, maxEndCol.w);
	}
}

void BasicVelGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		p->vel[i].x = LinearRand(minStartVel.x, maxStartVel.x);
		p->vel[i].y = LinearRand(minStartVel.y, maxStartVel.y);
		p->vel[i].z = LinearRand(minStartVel.z, maxStartVel.z);
		p->vel[i].w = LinearRand(minStartVel.w, maxStartVel.w);
	}
}

void SphereVelGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	float phi, theta, v, r;
	for (size_t i = startId; i < endId; ++i)
	{
		phi   = LinearRand(-M_PI, M_PI);
		theta = LinearRand(-M_PI, M_PI);
		v     = LinearRand(minVel, maxVel);

		r = v * sinf(phi);
		p->vel[i].z = v * cosf(phi);
		p->vel[i].x = r * cosf(theta);
		p->vel[i].y = r * sinf(theta);
	}
}

void VelFromPosGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		float scale = static_cast<float>(LinearRand(minScale, maxScale));
		p->vel[i].x = scale * (p->pos[i].x - offset.x);
		p->vel[i].y = scale * (p->pos[i].y - offset.y);
		p->vel[i].z = scale * (p->pos[i].z - offset.z);
		p->vel[i].w = scale * (p->pos[i].w - offset.w);
	}
}

void BasicTimeGen::Generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		p->time[i].x = p->time[i].y = LinearRand(minTime, maxTime);
		p->time[i].z = 0.f;
		p->time[i].w = 1.f / p->time[i].x;
	}
}

void BoxPosGen::ShowUI(bool& upt)
{
	ImVec4 _pos(pos.x, pos.y, pos.z, pos.w);
	if (UI::DragFloat4("Position##boxpos", &_pos.x))
	{
		pos = {_pos.x, _pos.y, _pos.z, _pos.w};
		upt = true;
	}
	ImVec4 _posOffset(maxStartPosOffset.x, maxStartPosOffset.y, maxStartPosOffset.z, maxStartPosOffset.w);
	if (UI::DragFloat4("Max Offset##boxpos", &_posOffset.x))
	{
		maxStartPosOffset = {_posOffset.x, _posOffset.y, _posOffset.z, _posOffset.w};
		upt = true;
	}
}

void SphereVelGen::ShowUI(bool &upt)
{
	if (UI::DragFloat("Min Vel##sphereVel", &minVel))
		upt = true;
	if (UI::DragFloat("Max Vel##sphereVel", &maxVel))
		upt = true;
}

void BasicVelGen::ShowUI(bool &upt)
{
	ImVec4 _minStartVel(minStartVel.x, minStartVel.y, minStartVel.z, minStartVel.w);
	if (UI::DragFloat4("Min Vel##basicvel", &_minStartVel.x))
	{
		minStartVel = {_minStartVel.x, _minStartVel.y, _minStartVel.z, _minStartVel.w};
		upt = true;
	}
	ImVec4 _maxStartVel(maxStartVel.x, maxStartVel.y, maxStartVel.z, maxStartVel.w);
	if (UI::DragFloat4("Max Vel##basicvel", &_maxStartVel.x))
	{
		maxStartVel = {_maxStartVel.x, _maxStartVel.y, _maxStartVel.z, _maxStartVel.w};
		upt = true;
	}
}

void BasicTimeGen::ShowUI(bool &upt)
{
	if (UI::DragFloat("Min Time##basictime", &minTime))
		upt = true;
	if (UI::DragFloat("Max Time##basictime", &maxTime))
		upt = true;
}

void SpherePosGen::ShowUI(bool &upt)
{
	ImVec4 _center(center.x, center.y, center.z, center.w);
	if (UI::DragFloat3("Center##circlepos", &_center.x))
	{
		center = {_center.x, _center.y, _center.z, _center.w};
		upt = true;
	}
	if (UI::DragFloat("X Radius##circlepos", &radX))
		upt = true;
	if (UI::DragFloat("Y Radius##circlepos", &radY))
		upt = true;
	if (UI::DragFloat("Z Radius##circlepos", &radZ))
		upt = true;
}

void VelFromPosGen::ShowUI(bool &upt)
{
	ImVec4 _offset(offset.x, offset.y, offset.z, offset.w);
	if (UI::DragFloat4("Center##circlepos", &_offset.x))
	{
		offset = {_offset.x, _offset.y, _offset.z, _offset.w};
		upt = true;
	}
	if (UI::DragFloat("Min Scale##velfrompos", &minScale))
		upt = true;
	if (UI::DragFloat("Max Scale##velfrompos", &maxScale))
		upt = true;
}

void BasicColorGen::ShowUI(bool &upt)
{
	ImVec4 _minStartCol(minStartCol.x, minStartCol.y, minStartCol.z, minStartCol.w);
	if (UI::DragFloat4("Start Min##col", &_minStartCol.x))
	{
		minStartCol = {_minStartCol.x, _minStartCol.y, _minStartCol.z, _minStartCol.w};
		upt = true;
	}
	ImVec4 _maxStartCol(maxStartCol.x, maxStartCol.y, maxStartCol.z, maxStartCol.w);
	if (UI::DragFloat4("Start Max##col", &_maxStartCol.x))
	{
		maxStartCol = {_maxStartCol.x, _maxStartCol.y, _maxStartCol.z, _maxStartCol.w};
		upt = true;
	}
	ImVec4 _minEndCol(minEndCol.x, minEndCol.y, minEndCol.z, minEndCol.w);
	if (UI::DragFloat4("End Min##col", &_minEndCol.x))
	{
		minEndCol = {_minEndCol.x, _minEndCol.y, _minEndCol.z, _minEndCol.w};
		upt = true;
	}
	ImVec4 _maxEndCol(maxEndCol.x, maxEndCol.y, maxEndCol.z, maxEndCol.w);
	if (UI::DragFloat4("End Max##col", &_maxEndCol.x))
	{
		maxEndCol = {_maxEndCol.x, _maxEndCol.y, _maxEndCol.z, _maxEndCol.w};
		upt = true;
	}
}

