#pragma once
#include <vector>
#include <algorithm>

#include "Rendering/FX/Particles/particleGenerator.hpp"
#include "Build/solidAPI.hpp"

namespace Solid
{

		class SOLID_API ParticleUpdater
		{
		public:
			ParticleUpdater() = default;

			virtual ~ParticleUpdater() = default;

			virtual void Update(float dt, ParticleData *p) = 0;
		};

		class SOLID_API EulerUpdater : public ParticleUpdater
		{
		public:
			Vec4 globalAcceleration;

			EulerUpdater() : globalAcceleration(0.f, 0.f, 0.f, 0.f)
			{}


			void ShowUI(bool& upt);
			void Update(float dt, ParticleData *p) override;
		};

		class SOLID_API FloorUpdater : public ParticleUpdater
		{
		public:
			float floorY;
			float bounceFactor;
		public:
			FloorUpdater() : floorY(0.0), bounceFactor(0.5f)
			{}


			void ShowUI(bool& upt);
			void Update(float dt, ParticleData *p) override;
		};

		class SOLID_API AttractorUpdater : public ParticleUpdater
		{
		public:
			std::vector<Vec4> attractors; // .w is force

			void ShowUI(bool& upt);
			void Update(float dt, ParticleData *p) override;

			size_t CollectionSize() const
			{ return attractors.size(); }

			void Add(const Vec4 &attr)
			{ attractors.push_back(attr); }

			Vec4 &Get(size_t id)
			{ return attractors[id]; }
		};

		class SOLID_API BasicColorUpdater : public ParticleUpdater
		{
		public:

			void Update(float dt, ParticleData *p) override;
		};

		class SOLID_API PosColorUpdater : public ParticleUpdater
		{
		public:
			Vec4 minPos;
			Vec4 maxPos;
		public:
			PosColorUpdater() : minPos(0.f, 0.f, 0.f, 0.f), maxPos(1.f, 1.f, 1.f, 1.f)
			{}


			void ShowUI(bool& upt);
			void Update(float dt, ParticleData *p) override;
		};

		class SOLID_API VelColorUpdater : public ParticleUpdater
		{
		public:
			Vec4 minVel;
			Vec4 maxVel;
		public:
			VelColorUpdater() : minVel(0.f, 0.f, 0.f, 0.f), maxVel(1.f, 1.f, 1.f, 1.f)
			{}


			void ShowUI(bool& upt);
			void Update(float dt, ParticleData *p) override;
		};

		class SOLID_API BasicTimeUpdater : public ParticleUpdater
		{
		public:

			void Update(float dt, ParticleData *p) override;
		};

	class SOLID_API KillerZoneUpdater : public ParticleUpdater
	{
	public:
		Vec3 pos;
		Vec3 offset;
	public:
		KillerZoneUpdater() : pos(0.f, 0.f, 0.f), offset(0.f, 0.f, 0.f)
		{}


		void ShowUI(bool& upt);
		void Update(float dt, ParticleData *p) override;
	};

}
