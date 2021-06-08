//
// Created by ryan1 on 02/06/2021.
//

#ifndef SOLIDLAUNCHER_ANIMATIONSYSTEM_HPP
#define SOLIDLAUNCHER_ANIMATIONSYSTEM_HPP
#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"

namespace Solid
{
	class SOLID_API AnimationSystem : public System
	{
	public:
		AnimationSystem(ECSManager& _ecsManager): System(_ecsManager){};

		void Update();
		void DebugUpdate(Camera& _camera);

	};
} //!namespace

#endif //SOLIDLAUNCHER_ANIMATIONSYSTEM_HPP
