#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/CameraMovement.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() CameraMovement : public Script
	{
	public:
		SLDField()
		float CamSpeed;
	private:
        float t = 0;
	    bool IsSecond = false;
        GameObject* camera1;
        GameObject* camera2;
        Engine* engine;

    public:
		CameraMovement();
		~CameraMovement();

		void Init()   override;
		void Update() override;
		CameraMovement_GENERATED
	};
	
	
	
}

File_GENERATED
