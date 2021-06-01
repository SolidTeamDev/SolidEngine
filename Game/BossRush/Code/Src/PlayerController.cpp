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
}

void PlayerController::Init()
{
    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
        rigidBody = &engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity());
    if(engine->ecsManager.GotComponent<Camera>(gameObject->GetEntity()))
        camera = &engine->ecsManager.GetComponent<Camera>(gameObject->GetEntity());
}

void PlayerController::Update()
{
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
}

void PlayerController::MoveForward()
{
    if(!rigidBody)
        return;

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

