//
// Created by ryan1 on 05/05/2021.
//

#include "ECS/System/scriptSystem.hpp"

using namespace Solid;

void ScriptSystem::Update()
{
	for (auto entity : entities)
	{
		Script* script = ecsManager.GetComponent<Script*>(entity);
		script->Update();
		//script->Update();
		//script->Update();
	}

}
