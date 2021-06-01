#include "ECS/Components/scriptList.hpp"
#include "ECS/System/scriptSystem.hpp"

using namespace Solid;

void ScriptSystem::Init()
{
    for (auto entity : entities)
    {
        ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(entity);


        scriptL.Init();

    }

}

void ScriptSystem::Destroy()
{
    for (auto entity : entities)
    {
        ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(entity);


        scriptL.Destroy();

    }

}

void ScriptSystem::Update()
{
	for (auto entity : entities)
	{
		ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(entity);


		scriptL.Update();

	}

}

void ScriptSystem::FixedUpdate()
{
    for (auto entity : entities)
    {
        ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(entity);


        scriptL.FixedUpdate();

    }
}

void ScriptSystem::LateUpdate()
{
    for (auto entity : entities)
    {
        ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(entity);


        scriptL.LateUpdate();

    }
}
