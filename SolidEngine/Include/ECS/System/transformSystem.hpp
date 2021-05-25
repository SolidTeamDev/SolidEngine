//
// Created by ryan1 on 21/05/2021.
//

#ifndef SOLIDLAUNCHER_TRANSFORMSYSTEM_HPP
#define SOLIDLAUNCHER_TRANSFORMSYSTEM_HPP


#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"



namespace Solid
{

	class SOLID_API TransformSystem : public System
	{
	public:
		TransformSystem(ECSManager& _ecsManager): System(_ecsManager){};

		void Update();
		void FixedUpdate();
		void LateUpdate();
	};
}


#endif //SOLIDLAUNCHER_TRANSFORMSYSTEM_HPP
