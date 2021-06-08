#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/GameManager.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() GameManager : public Script
	{
	public:
		
	private:
		SLDField()
		int MaxEntityNum = 1;
		int CurrentNumOfEntity = 0;
		SLDField()
		String PrefabName = "";
		SLDField()
		float ForceProjection = 0;
		SLDField()
		float maxCounter = 1;
		float counter = -1;

	public:
		GameManager();
		~GameManager();
		
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		GameManager_GENERATED
	};
	
	
	
}

File_GENERATED
