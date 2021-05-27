#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/PlayerController.sld.hpp"
namespace Solid SLDNamespace()
{
    class Engine;
	class SOLID_SCRIPT_API SLDClass() PlayerController : public Script
	{
	public:
        SLDField()
        float moveSpeed;
        SLDField()
        float jumpForce;
        SLDField()
        int health;
	private:
		Engine* engine = nullptr;
	    RigidBody* rigidBody = nullptr;

	public:
		PlayerController();
		~PlayerController() override = default;

		void Init() override;

		void Update() override;

	private:

	    void MoveForward();
	    void MoveBack();
	    void MoveLeft();
	    void MoveRight();
	    void Jump();

		PlayerController_GENERATED
	};
	
	
	
}

File_GENERATED
