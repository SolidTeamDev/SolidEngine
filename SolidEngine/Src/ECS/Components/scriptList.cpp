//
// Created by ryan1 on 10/05/2021.
//

#include "ECS/Components/scriptList.hpp"


using namespace Solid;

ScriptList::ScriptList()
{

}

ScriptList::~ScriptList()
{

}


Script *ScriptList::AddScript( Script *_script)
{

	for(Script* elt : Scripts)
	{
		if(elt == _script || elt->getArchetype().name == _script->getArchetype().name)
		{
			return elt;
		}
	}

	{
		_script->gameObject = gameObject;

		Scripts.push_back(_script);
		return _script;
		//add script
	}

}

Script *ScriptList::AddScript(const char *_scriptName)
{
	for(Script* elt : Scripts)
	{
		if(elt->getArchetype().name == _scriptName)
		{
			return elt;
		}
	}
	return nullptr;
}



bool ScriptList::HasScript(const char *_scriptName)
{
	for(Script* elt : Scripts)
	{
		if(elt->getArchetype().name == _scriptName)
			return true;
	}
	return false;
}

bool ScriptList::HasScript(const Script *_script)
{

	for(Script* elt : Scripts)
	{
	    if(elt == _script)
	    	return true;
	}
	return false;
}



void ScriptList::RemoveScript(const char *_scriptName)
{
	for(Script* elt : Scripts)
	{
	    if(elt->getArchetype().name == _scriptName)
	    {
	    	//remove
		    break;
	    }
	}
}

Script *ScriptList::GetScript(const char *_scriptName)
{
	for(Script* elt : Scripts)
	{
		if(elt->getArchetype().name == _scriptName)
		{
			return elt;
		}
	}
	return nullptr;
}

void ScriptList::Update()
{
	for(Script* elt : Scripts)
	{
	    elt->Update();
	}
}

std::vector<Script *> &ScriptList::GetAllScripts()
{
	return Scripts;
}

void ScriptList::CleanAllNullptr()
{
	int i = 0;
	for(auto it = Scripts.begin(); it != Scripts.end(); ++it)
	{
	    if((*it) == nullptr)
	    {
	    	Scripts.erase(it);
		    it = Scripts.begin() +((i > 0 ? i:1)-1);
	    }
	    else
	    	++i;
	}
}



