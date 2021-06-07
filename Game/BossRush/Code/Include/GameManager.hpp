#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "Core/engine.hpp"
#include "EngineGenerated/GameManager.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() GameManager : public Script
	{
	public:

	private:
		Engine* engine = nullptr;
	public:
		GameManager();
		~GameManager();

		void Init() override;

        void Destroy() override;

        void Update() override;

		GameManager_GENERATED
	};
	
	
	
}

File_GENERATED
