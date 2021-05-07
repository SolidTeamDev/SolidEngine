//
// Created by ryan1 on 05/05/2021.
//

#ifndef SOLIDLAUNCHER_SCRIPTSYSTEM_HPP
#define SOLIDLAUNCHER_SCRIPTSYSTEM_HPP
#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"
#include "ECS/Components/camera.hpp"
#include "ECS/Components/script.hpp"

namespace Solid
{
	class SOLID_API ScriptSystem : public System
	{
	public:
		ScriptSystem(ECSManager& _ecsManager): System(_ecsManager){};

		void Update();
	};
} //!namespace
#endif //SOLIDLAUNCHER_SCRIPTSYSTEM_HPP
