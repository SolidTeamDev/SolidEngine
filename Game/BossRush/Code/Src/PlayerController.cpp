#include "PlayerController.hpp"

#include <Core/engine.hpp>

using namespace Solid;

PlayerController::PlayerController()
{
    engine = Engine::GetInstance();
    engine->inputManager->AddKeyInput("Forward",GLFW_KEY_W,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Back",GLFW_KEY_S,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Left",GLFW_KEY_A,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Right",GLFW_KEY_D,ImEnumDetectionType::PRESSED);
    engine->inputManager->AddKeyInput("Jump",GLFW_KEY_SPACE, ImEnumDetectionType::NO_REPEAT);
    engine->inputManager->AddMouseInput("Fire1",GLFW_MOUSE_BUTTON_1, ImEnumDetectionType::NO_REPEAT);

    engine->inputManager->AddKeyInput("Pause",GLFW_KEY_ESCAPE, ImEnumDetectionType::NO_REPEAT);
}

void PlayerController::Init()
{
    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
        rigidBody = &engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity());

    camera = engine->activeCamera->gameObject;

    engine->inputManager->ShowCursor(false);
}

void PlayerController::Destroy()
{
    engine->inputManager->ShowCursor(true);
}

void PlayerController::Update()
{
    RotateCamera();

    if(engine->inputManager->IsPressed("Forward"))
        MoveForward();
    if(engine->inputManager->IsPressed("Back"))
        MoveBack();
    if(engine->inputManager->IsPressed("Left"))
        MoveLeft();
    if(engine->inputManager->IsPressed("Right"))
        MoveRight();
    if(engine->inputManager->IsPressed("Jump") && isGrounded)
        Jump();

    if(engine->inputManager->IsPressed("Fire1"))
        Fire();

    if(engine->inputManager->IsPressed("Pause"))
    {
        isPaused = !isPaused;
        engine->inputManager->ShowCursor(isPaused);
    }
}

void PlayerController::RotateCamera()
{
    if(!camera || isPaused)
        return;

    auto& transform = engine->ecsManager.GetComponent<Transform>(camera->parent->GetEntity());

    Vec3 rot = transform.GetLocalEuler();
    double xpos = 0, ypos = 0;
    engine->inputManager->GetCursorPos(xpos,ypos);

    xpos = Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().x/2) - (int)xpos);
    ypos = Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().y/2) - (int)ypos);

    engine->inputManager->SetCursorPos(engine->window->GetWindowSize().x*0.5,
                                       engine->window->GetWindowSize().y*0.5);

    //rot.x += (float)ypos * mouseSensitivity;
    rot.y -= (float)xpos * mouseSensitivity;


    if (rot.x < -120.0f)
        rot.x = -120.0f;
    if (rot.x > -63.0f)
        rot.x = -63.0f;

    transform.SetEuler(rot);
}

void PlayerController::MoveForward()
{
    if(!rigidBody)
        return;

    Vec3 camRot = camera->parent->transform->GetLocalEuler();
    Vec3 playerRot = gameObject->transform->GetLocalEuler();

    camera->parent->transform->SetEuler(Vec3(0));

    gameObject->transform->SetEuler(Vec3(playerRot.x,playerRot.y,camRot.y));

    rigidBody->AddForce(Vec3(0,0,moveSpeed));
}

void PlayerController::MoveBack()
{
    if(!rigidBody)
        return;

    rigidBody->AddForce(Vec3(0,0,-moveSpeed));
}

void PlayerController::MoveLeft()
{
    if(!rigidBody)
        return;

    rigidBody->AddForce(Vec3(moveSpeed,0,0));
}

void PlayerController::MoveRight()
{
    if(!rigidBody)
        return;

    rigidBody->AddForce(Vec3(-moveSpeed,0,0));
}

void PlayerController::Jump()
{
    if(!rigidBody)
        return;

    rigidBody->AddImpulse(Vec3(0,jumpForce,0));
}

void PlayerController::Fire()
{
    /*GameObject* go = engine->ecsManager.Instantiate("Bullet", nullptr, "PlayerBullet");

    go->transform->SetPosition(gameObject->transform->GetPosition());
    engine->ecsManager.GetComponent<RigidBody>(go->GetEntity()).SetLinearVelocity(Vec3(0,0,10));*/
}

void PlayerController::OnContactEnter(GameObject *_other)
{
    if(_other->name == "Ground")
        isGrounded = true;
}

void PlayerController::OnContactExit(GameObject *_other)
{
    if(_other->name == "Ground")
        isGrounded = false;
}

