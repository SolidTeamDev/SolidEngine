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
    size_t i = 0;
	for (auto it = entities.begin() ; it != entities.end();)
	{
		ScriptList& scriptL = ecsManager.GetComponent<ScriptList>(*it);


		scriptL.Update();

        if(ecsManager.GetCompArray<ScriptList>()->IsEntityDestroyed() != -1)
        {
            it = entities.begin();
            for(int j = 0 ; j < i ; ++j)
                it++;

            if(it == entities.end())
                break;
        }
        else
            it++;

        ++i;
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
