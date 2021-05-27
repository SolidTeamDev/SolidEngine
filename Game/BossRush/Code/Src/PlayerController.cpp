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
}

void PlayerController::Init()
{
    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
        rigidBody = &engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity());
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
    if(engine->inputManager->IsPressed("Jump"))
        Jump();
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

