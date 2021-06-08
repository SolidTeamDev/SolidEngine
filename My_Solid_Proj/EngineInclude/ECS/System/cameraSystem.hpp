//
// Created by ryan1 on 28/05/2021.
//

#ifndef SOLIDLAUNCHER_CAMERASYSTEM_HPP
#define SOLIDLAUNCHER_CAMERASYSTEM_HPP


#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"

namespace Solid
{
	class SOLID_API CameraSystem : public System
	{
	public:
		CameraSystem(ECSManager& _ecsManager): System(_ecsManager){};

		void Update();
	};
}


#endif //SOLIDLAUNCHER_CAMERASYSTEM_HPP
