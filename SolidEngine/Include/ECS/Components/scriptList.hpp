//
// Created by ryan1 on 10/05/2021.
//

#ifndef SOLIDLAUNCHER_SCRIPTLIST_HPP
#define SOLIDLAUNCHER_SCRIPTLIST_HPP


#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "script.hpp"
#include "EngineGenerated/scriptList.sld.hpp"

namespace Solid SLDNamespace()
{

	class SOLID_API SLDClass() ScriptList  : public Components
	{
	public:
	//public members

	protected:
	//protected members
		std::vector<Script*> Scripts;



	public:
	//public func
	    ScriptList ();
	    ~ScriptList () override;

		Script* AddScript(Script* _script);
		Script* AddScript(const char* _scriptName);


		bool HasScript(const char* _scriptName);
		bool HasScript(const Script* _script);


		Script* GetScript(const char* _scriptName);
		std::vector<Script*>& GetAllScripts();

		void CleanAllNullptr();
		void RemoveScript(const char* _scriptName);
		void RemoveScript(Script* _script);
		void Update();
		void FixedUpdate();
		void LateUpdate();

		virtual void Init()override
		{
            for (int i = 0; i < Scripts.size(); ++i)
            {
                Script* elt = Scripts[i];
                elt->Init();
            }
		} ;
		virtual void Release()override
		{
			Components::Release();
			for (int i = 0; i < Scripts.size(); ++i)
			{
				Script* elt = Scripts[i];
				delete elt;
				Scripts[i] = nullptr;

			}
			Scripts.clear();
		};

        virtual void Destroy()
        {
            for (int i = 0; i < Scripts.size(); ++i)
            {
                Script* elt = Scripts[i];
                elt->Destroy();
            }
        } ;

		ScriptList_GENERATED

	};

}

File_GENERATED



#endif //SOLIDLAUNCHER_SCRIPTLIST_HPP
