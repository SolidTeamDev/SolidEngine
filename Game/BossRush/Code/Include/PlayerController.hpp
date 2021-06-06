#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/camera.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/PlayerController.sld.hpp"
namespace Solid SLDNamespace()
{
    class Engine;
	class SOLID_SCRIPT_API SLDClass() PlayerController : public Script
	{
	public:
        SLDField()
        float moveSpeed = 0;
        SLDField()
        float jumpForce = 0;
        SLDField()
        int health = 0;

        SLDField()
        float bulletSpeed = 0;

        SLDField()
        float mouseSensitivity = 0;

        SLDField()
        String animIdle = "";
        SLDField()
        String animFwd = "";
	private:
		Engine* engine = nullptr;
	    RigidBody* rigidBody = nullptr;
	    GameObject* camera = nullptr;
	    GameObject* mesh = nullptr;

	    bool isGrounded = false;

	    bool isPaused = false;//Temporary

	public:
		PlayerController();
		~PlayerController() override = default;

		void Init() override;

        void Destroy() override;

        void Update() override;

        void OnContactEnter(GameObject *_other) override;

        void OnContactExit(GameObject *_other) override;

    private:

	    void RotateCamera();
	    void MoveForward();
	    void MoveBack();
	    void MoveLeft();
	    void MoveRight();
	    void Jump();
	    void Dash();

	    void Fire();

		PlayerController_GENERATED
	};
	
	
	
}

File_GENERATED
