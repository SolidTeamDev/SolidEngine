//
// Created by ryan1 on 05/05/2021.
//
#include "ECS/Components/scriptList.hpp"
#include "ECS/System/scriptSystem.hpp"

using namespace Solid;

void ScriptSystem::Update()
{
	for (auto entity : entities)
	{
		ScriptList scriptL = ecsManager.GetComponent<ScriptList>(entity);


		scriptL.Update();

	}

}
