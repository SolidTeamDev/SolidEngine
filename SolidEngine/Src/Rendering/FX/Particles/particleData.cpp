#include "Rendering/FX/Particles/particleData.hpp"

using namespace Solid::Particles;

ParticleData::ParticleData(size_t maxCount)
{
	Generate(maxCount);
}

void ParticleData::Generate(size_t maxSize)
{
	count      = maxSize;
	countAlive = 0;
	pos =std::make_unique<Vec4[]>(maxSize);
	col=std::make_unique<Vec4[]>(maxSize);
	vel=std::make_unique<Vec4[]>(maxSize);
	acc=std::make_unique<Vec4[]>(maxSize);
	alive=std::make_unique<bool[]>(maxSize);
	time=std::make_unique<Vec4[]>(maxSize);
	endCol=std::make_unique<Vec4[]>(maxSize);
	startCol=std::make_unique<Vec4[]>(maxSize);
}

void ParticleData::Kill(size_t ID)
{
	if (countAlive > 0)
	{
		alive[ID] = false;
		countAlive --;
		SwapData(ID, countAlive);
	}
}

void ParticleData::Wake(size_t ID)
{
	if (countAlive < count)
	{
		bool oldState = alive[ID];
		alive[ID] = true;
		SwapData(ID, countAlive);
		countAlive += oldState;
	}
}

void ParticleData::SwapData(size_t a, size_t b)
{
	std::swap(startCol[a], startCol[b]);
	std::swap(endCol[a], endCol[b]);
	std::swap(alive[a], alive[b]);
	std::swap(time[a], time[b]);
	std::swap(vel[a], vel[b]);
	std::swap(acc[a], acc[b]);
	std::swap(col[a], col[b]);
	std::swap(pos[a], pos[b]);
}

