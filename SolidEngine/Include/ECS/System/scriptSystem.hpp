#pragma once

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

		void Init();
		void Update();
		void FixedUpdate();
		void LateUpdate();
	};
} //!namespace
