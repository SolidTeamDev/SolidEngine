#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/animation.hpp"
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
        float cameraDistance = 0;

        String animIdle = "ANIM_Prisme_Idle.FBX";
        String animFwd = "ANIM_Prisme_Jog_Fwd.FBX";
        String animFwdEnd = "ANIM_Prisme_Jog_Fwd_Stop.FBX";
        String animBwdEnd = "ANIM_Prisme_Jog_Bwd_Stop.FBX";
        String animBwd = "ANIM_Prisme_Jog_Bwd.FBX";
        String animJumpEnd = "ANIM_Prisme_Jump_Recovery.FBX";
        String animJumpStart = "ANIM_Prisme_Jump_Start.FBX";
        String animAttackA = "ANIM_Prisme_Primary_Attack_A_Fast.FBX";
        String animAttackB = "ANIM_Prisme_Primary_Attack_B_Fast.FBX";
        String animChargeAttack = "ANIM_Prisme_Ability_E.FBX";
        String animDeath = "ANIM_Prisme_Death.FBX";
        String animTaunt = "ANIM_Prisme_Recall.FBX";

    private:
	    enum class EStateAnim
	    {
	        Run,
	        Idle,
	        Back,
	        Right,
	        Left,
	        Attack1,
	        Attack2,
	        AttackCharge,
	        JumpStart,
	        JumpEnd,
	        Taunt,
	        Death,

	        None
	    };
	    EStateAnim stateAnim = EStateAnim::None;
		Engine* engine = nullptr;
	    RigidBody* rigidBody = nullptr;
	    GameObject* camera = nullptr;
	    GameObject* mesh = nullptr;
        Animation* anim;
	    bool isGrounded = false;

	    bool isPaused = false;//Temporary

	public:
		PlayerController();
		~PlayerController();

		void Init() override;

        void Destroy() override;

        void Update() override;

        void LateUpdate() override;

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
	    void Dead();

	    void Fire();

		PlayerController_GENERATED
	};
	
	
	
}

File_GENERATED
