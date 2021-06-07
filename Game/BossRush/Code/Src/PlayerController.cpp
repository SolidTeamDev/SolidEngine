#include "PlayerController.hpp"
#include "followingObj.hpp"
#include "ECS/Components/scriptList.hpp"
#include <iostream>
#include <Core/engine.hpp>

using namespace Solid;

PlayerController::PlayerController()
{
    engine = Engine::GetInstance();
    engine->inputManager->AddKeyInput("Forward",GLFW_KEY_W,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Back",GLFW_KEY_S,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Left",GLFW_KEY_A,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Right",GLFW_KEY_D,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Dash",GLFW_KEY_LEFT_SHIFT, ImEnumDetectionType::NO_REPEAT);
    engine->inputManager->AddKeyInput("Jump",GLFW_KEY_SPACE, ImEnumDetectionType::NO_REPEAT);
    engine->inputManager->AddMouseInput("Fire1",GLFW_MOUSE_BUTTON_1, ImEnumDetectionType::NO_REPEAT);
    engine->inputManager->AddMouseInput("Fire2",GLFW_MOUSE_BUTTON_2, ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Taunt",GLFW_KEY_T, ImEnumDetectionType::NO_REPEAT);

    engine->inputManager->AddKeyInput("Pause",GLFW_KEY_ESCAPE, ImEnumDetectionType::NO_REPEAT);

}
PlayerController::~PlayerController()
{

}

void PlayerController::Init()
{
    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
        rigidBody = &engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity());


    camera = engine->activeCamera->gameObject;
    mesh = engine->ecsManager.FindGameObjectByName("Mesh",gameObject->childs[0]);
    if(engine->ecsManager.GotComponent<Animation>(mesh->GetEntity()))
        anim = &engine->ecsManager.GetComponent<Animation>(mesh->GetEntity());
    engine->inputManager->ShowCursor(false);
}

void PlayerController::Destroy()
{
    engine->inputManager->ShowCursor(true);
}

void PlayerController::Update()
{
    if (engine->inputManager->IsPressed("Pause"))
    {
        isPaused = !isPaused;
        engine->inputManager->ShowCursor(isPaused);
    }

    RotateCamera();

    if(health <= 0 && stateAnim != EStateAnim::Death)
    {
        Dead();
        return;
    }

    //Controller and Animation
    if(stateAnim != EStateAnim::Death)
    {
        if (engine->inputManager->IsPressed("Forward"))
            MoveForward();
        if (engine->inputManager->IsReleased("Forward"))
        {
            if (stateAnim == EStateAnim::Run)
            {
                anim->SetAnim(animIdle, true);
                stateAnim = EStateAnim::Idle;
            }
        }
        if (engine->inputManager->IsPressed("Back"))
            MoveBack();
        if (engine->inputManager->IsReleased("Back"))
        {

            if (stateAnim == EStateAnim::Back)
            {
                anim->SetAnim(animIdle, true);
                stateAnim = EStateAnim::Idle;
            }
        }
        if (engine->inputManager->IsPressed("Left"))
            MoveLeft();
        if (engine->inputManager->IsReleased("Left"))
        {
            if (stateAnim == EStateAnim::Left)
            {
                anim->SetAnim(animIdle, true);
                stateAnim = EStateAnim::Idle;
            }
        }
        if (engine->inputManager->IsPressed("Right"))
            MoveRight();
        if (engine->inputManager->IsReleased("Right"))
        {
            if (stateAnim == EStateAnim::Right)
            {
                anim->SetAnim(animIdle, true);
                stateAnim = EStateAnim::Idle;
            }
        }
        if (engine->inputManager->IsPressed("Jump") && isGrounded)
            Jump();
        if (engine->inputManager->IsPressed("Dash"))
            Dash();

        if (engine->inputManager->IsPressed("Fire1"))
        {
            String nameAnim = "";
            if (stateAnim != EStateAnim::Attack1 && stateAnim != EStateAnim::Attack2)
            {
                stateAnim = EStateAnim::Attack1;
                nameAnim = animAttackA;
            } else if (stateAnim == EStateAnim::Attack1)
            {
                stateAnim = EStateAnim::Attack2;
                nameAnim = animAttackB;

            } else if (stateAnim == EStateAnim::Attack2)
            {
                stateAnim = EStateAnim::Attack1;
                nameAnim = animAttackA;
            }
            anim->SetAnim(nameAnim, false);

            Fire();
        }

        if (engine->inputManager->IsPressed("Fire2"))
        {
            if (mesh && stateAnim != EStateAnim::AttackCharge && stateAnim != EStateAnim::JumpStart)
            {
                anim->SetAnim(animChargeAttack, false,0.75);
                stateAnim = EStateAnim::AttackCharge;
            }
        }
        if (engine->inputManager->IsPressed("Taunt"))
        {
            if (mesh && stateAnim == EStateAnim::Idle)
            {
                anim->SetAnim(animTaunt, false);
            }
        }

        if (stateAnim == EStateAnim::AttackCharge || stateAnim == EStateAnim::Attack1 ||
            stateAnim == EStateAnim::Attack2 || stateAnim == EStateAnim::JumpEnd)
        {
            if (anim->IsFinish)
            {
                anim->SetAnim(animIdle, true);
                stateAnim = EStateAnim::Idle;
            }
        }
    }
}
void PlayerController::LateUpdate()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking;
	UI::SetNextWindowPos(ImVec2(10,10));
	UI::SetNextWindowSize(ImVec2(500,100));
	float f = (float)health/ 100.0f;
	UI::Begin("BAR", nullptr,flags);
	std::string s = std::to_string(health) + "/ 100 ";
	UI::ProgressBar(f, ImVec2(300,15), s.data());
	UI::End();
}
void PlayerController::RotateCamera()
{
    if(!camera || isPaused)
        return;

    auto& transform = engine->ecsManager.GetComponent<Transform>(camera->GetEntity());

    Vec3 rot = transform.GetGlobalEuler();
    double xpos = 0, ypos = 0;
    engine->inputManager->GetCursorPos(xpos,ypos);

    xpos = Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().x/2) - (int)xpos);
    ypos = Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().y/2) - (int)ypos);

    engine->inputManager->SetCursorPos(engine->window->GetWindowSize().x*0.5,
                                       engine->window->GetWindowSize().y*0.5);

    rot.x += (float)ypos * mouseSensitivity;
    rot.y -= (float)xpos * mouseSensitivity;
    rot.z = 180;


    if (rot.x < -60)
        rot.x = -60;
    if (rot.x > 10)
        rot.x = 10;

    transform.SetPosition(Vec3(0,0,0));
    transform.SetEuler(rot);
    transform.SetPosition(transform.GetLocalForward() * cameraDistance);
}

void PlayerController::MoveForward()
{
    if((stateAnim == EStateAnim::Attack1 ||stateAnim == EStateAnim::Attack2) && !anim->IsFinish)
    {
    }
    else if(stateAnim == EStateAnim::AttackCharge && !anim->IsFinish)
    {
        return;
    }
    else if(mesh && stateAnim != EStateAnim::Run && stateAnim != EStateAnim::JumpStart)
    {
        anim->SetAnim(animFwd, true);
        stateAnim = EStateAnim::Run;
    }

    Vec3 dir = camera->transform->GetLocalForward() * moveSpeed * Time::DeltaTime();

    dir.y = 0;

    gameObject->transform->Translate(dir);

    mesh->transform->SetEuler(Vec3(90,0,camera->transform->GetLocalEuler().y));
}

void PlayerController::MoveBack()
{
    if((stateAnim == EStateAnim::Attack1 ||stateAnim == EStateAnim::Attack2) && !anim->IsFinish)
    {
    }
    else if(stateAnim == EStateAnim::AttackCharge && !anim->IsFinish)
    {
        return;
    }
    else if(mesh && stateAnim != EStateAnim::Back && stateAnim != EStateAnim::JumpStart)
    {
        anim->SetAnim(animBwd, true);
        stateAnim = EStateAnim::Back;
    }

    Vec3 dir = camera->transform->GetLocalForward() * -(moveSpeed/2) * Time::DeltaTime();

    dir.y = 0;

    gameObject->transform->Translate(dir);
    mesh->transform->SetEuler(Vec3(90,0,camera->transform->GetLocalEuler().y));
}

void PlayerController::MoveLeft()
{
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalRight() * moveSpeed * Time::DeltaTime();

    dir.y = 0;
    if(stateAnim == EStateAnim::AttackCharge && !anim->IsFinish)
    {
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y));
        return;
    }

    gameObject->transform->Translate(dir);

    if((stateAnim == EStateAnim::Attack1 ||stateAnim == EStateAnim::Attack2) && !anim->IsFinish)
        mesh->transform->SetEuler(Vec3(90,0,camera->transform->GetLocalEuler().y));
    else if (stateAnim == EStateAnim::Back)
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y + 45.f));
    else if (stateAnim == EStateAnim::Run)
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y - 45.f));
    else if (mesh && stateAnim != EStateAnim::JumpStart && stateAnim != EStateAnim::Left)
    {
        anim->SetAnim(animFwd, true);
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y - 90.f));
        stateAnim = EStateAnim::Left;
    }

}

void PlayerController::MoveRight()
{
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalRight() * -moveSpeed * Time::DeltaTime();

    dir.y = 0;

    if(stateAnim == EStateAnim::AttackCharge && !anim->IsFinish)
    {
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y));

        return;
    }

    gameObject->transform->Translate(dir);

    if((stateAnim == EStateAnim::Attack1 ||stateAnim == EStateAnim::Attack2) && !anim->IsFinish)
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y));
    else if (stateAnim == EStateAnim::Back)
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y - 45.f));
    else if (stateAnim == EStateAnim::Run)
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y + 45.f));
    else if (mesh && stateAnim != EStateAnim::JumpStart && stateAnim != EStateAnim::Right)
    {
        anim->SetAnim(animFwd, true);
        mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y + 90.f));
        stateAnim = EStateAnim::Right;
    }

}

void PlayerController::Jump()
{
    if(!rigidBody)
        return;
    if(mesh && stateAnim != EStateAnim::JumpStart)
    {
        anim->SetAnim(animJumpStart, false);
        stateAnim = EStateAnim::JumpStart;
    }
    rigidBody->AddImpulse(Vec3(0,jumpForce,0));
}

void PlayerController::Dash()
{
    
}

void PlayerController::Fire()
{
    Vec3 dir = camera->transform->GetLocalForward() * bulletSpeed;

    mesh->transform->SetEuler(Vec3(90, 0, camera->transform->GetLocalEuler().y));

    GameObject* go = engine->ecsManager.Instantiate("Bullet", nullptr, "PlayerBullet");
    go->transform->SetPosition(gameObject->transform->GetLocalPosition());
    engine->ecsManager.GetComponent<RigidBody>(go->GetEntity()).SetLinearVelocity(dir);


}
void PlayerController::Dead()
{
    anim->SetAnim(animDeath, false);
    stateAnim = EStateAnim::Death;
}
void PlayerController::OnContactEnter(GameObject *_other)
{
    if(_other->name == "Ground")
    {
        if(mesh && stateAnim != EStateAnim::JumpEnd && stateAnim == EStateAnim::JumpStart)
        {
            anim->SetAnim(animJumpEnd, false);
            stateAnim = EStateAnim::JumpEnd;
        }
        isGrounded = true;
    }
}

void PlayerController::OnContactExit(GameObject *_other)
{
    if(_other->name == "Ground")
        isGrounded = false;
}

void PlayerController::OnTriggerEnter(GameObject *_other)
{
    if(_other->tag != "CubeBullet")
        return;

    health -= 5;
}

