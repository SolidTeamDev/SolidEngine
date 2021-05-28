#pragma once

#include "Rendering/FX/Particles/particleData.hpp"

namespace Solid
{
	namespace Particles
	{
		class SOLID_API ParticleGenerator
		{
		public:
			ParticleGenerator() = default;

			virtual ~ParticleGenerator() = default;

			virtual void Generate(double dt, ParticleData *p, size_t start, size_t end) = 0;
		};

		class SOLID_API BoxPosGen : public ParticleGenerator
		{
		public:

			Vec4 pos;
			Vec4 maxStartPosOffset;

			BoxPosGen() : pos(0.f, 0.f, 0.f, 0.f), maxStartPosOffset(0.f, 0.f, 0.f, 0.f)
			{}

			BoxPosGen(const Vec4 &_pos, const Vec4 &_offset) :
					pos(_pos),
					maxStartPosOffset(_offset)
			{};


			void Generate(double dt, ParticleData *p, size_t start, size_t end) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API CirclePosGen : public ParticleGenerator
		{
		public:

			Vec4 center;
			float radX;
			float radY;

			CirclePosGen() : center(0.f, 0.f, 0.f, 0.f), radX(0.f), radY(0.f)
			{};

			CirclePosGen(const Vec4 &_center, float _radX, float _radY) :
					center(_center),
					radX(_radX),
					radY(_radY)
			{};

			void Generate(double dt, ParticleData *p, size_t start, size_t end) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API BasicColorGen : public ParticleGenerator
		{
		public:
			Vec4 minStartCol;
			Vec4 maxStartCol;
			Vec4 minEndCol;
			Vec4 maxEndCol;

			BasicColorGen() : minStartCol(0.f, 0.f, 0.f, 0.f), maxStartCol(0.f, 0.f, 0.f, 0.f),
			                  minEndCol(0.f, 0.f, 0.f, 0.f), maxEndCol(0.f, 0.f, 0.f, 0.f)
			{};

			void Generate(double dt, ParticleData *p, size_t startId, size_t endId) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API BasicVelGen : public ParticleGenerator
		{
		public:
			Vec4 minStartVel;
			Vec4 maxStartVel;

			BasicVelGen() : minStartVel(0.f, 0.f, 0.f, 0.f), maxStartVel(0.f, 0.f, 0.f, 0.f)
			{}

			void Generate(double dt, ParticleData *p, size_t startId, size_t endId) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API SphereVelGen : public ParticleGenerator
		{
		public:
			float minVel;
			float maxVel;

			SphereVelGen() : minVel(0.0), maxVel(0.0)
			{}

			void Generate(double dt, ParticleData *p, size_t startId, size_t endId) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API VelFromPosGen : public ParticleGenerator
		{
		public:
			Vec4 offset;
			float minScale;
			float maxScale;

			VelFromPosGen() : offset(0.f, 0.f, 0.f, 0.f), minScale(0.0), maxScale(0.0)
			{}

			VelFromPosGen(const Vec4 &off, double minS, double maxS)
					: offset(off), minScale((float) minS), maxScale((float) maxS)
			{}

			void Generate(double dt, ParticleData *p, size_t startId, size_t endId) override;
			void ShowUI(bool& upt);
		};

		class SOLID_API BasicTimeGen : public ParticleGenerator
		{
		public:
			float minTime;
			float maxTime;

			BasicTimeGen() : minTime(0.0), maxTime(0.0)
			{}

			void Generate(double dt, ParticleData *p, size_t startId, size_t endId) override;
			void ShowUI(bool& upt);
		};
	}
}
