#pragma once

#include <memory>

#include "Core/Maths/solidVector.hpp"


namespace Solid
{
	class ParticleData
	{
	public:
		std::unique_ptr<Vec4[]> startCol;
		std::unique_ptr<Vec4[]> endCol;
		std::unique_ptr<Vec4[]> time;
		std::unique_ptr<Vec4[]> pos;
		std::unique_ptr<Vec4[]> col;
		std::unique_ptr<Vec4[]> vel;
		std::unique_ptr<Vec4[]> acc;
		std::unique_ptr<bool[]> alive;

		size_t count = 0;
		size_t countAlive = 0;

		ParticleData() = default;

		explicit ParticleData(size_t maxCount);

		~ParticleData() = default;

		ParticleData(const ParticleData &) = delete;

		ParticleData &operator=(const ParticleData &) = delete;

		void SwapData(size_t a, size_t b);

		void Generate(size_t maxSize);

		void Kill(size_t ID);

		void Wake(size_t ID);

		static size_t ComputeMemoryUsage(const ParticleData &p)
		{
			return p.count * (7 * sizeof(Vec4) + sizeof(bool)) + sizeof(size_t) * 2;
		}
	};
}
