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

    engine->inputManager->AddKeyInput("Pause",GLFW_KEY_ESCAPE, ImEnumDetectionType::NO_REPEAT);
}

void PlayerController::Init()
{
    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
        rigidBody = &engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity());

    camera = engine->activeCamera->gameObject;
    mesh = engine->ecsManager.FindGameObjectByName("Mesh",gameObject->childs[0]);

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
    if(engine->inputManager->IsPressed("Dash"))
        Dash();

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

    auto& transform = engine->ecsManager.GetComponent<Transform>(camera->GetEntity());

    Vec3 rot = transform.GetLocalEuler();
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
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalForward() * moveSpeed;

    dir.y = 0;

    rigidBody->AddForce(dir);

    mesh->transform->SetEuler(Vec3(90,0,camera->parent->transform->GetLocalEuler().y));
}

void PlayerController::MoveBack()
{
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalForward() * -moveSpeed;

    dir.y = 0;

    rigidBody->AddForce(dir);
}

void PlayerController::MoveLeft()
{
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalRight() * moveSpeed;

    dir.y = 0;

    rigidBody->AddForce(dir);
}

void PlayerController::MoveRight()
{
    if(!rigidBody)
        return;

    Vec3 dir = camera->transform->GetLocalRight() * -moveSpeed;

    dir.y = 0;

    rigidBody->AddForce(dir);
}

void PlayerController::Jump()
{
    if(!rigidBody)
        return;

    rigidBody->AddImpulse(Vec3(0,jumpForce,0));
}

void PlayerController::Dash()
{
    
}

void PlayerController::Fire()
{
    Vec3 dir = camera->transform->GetLocalForward() * bulletSpeed;
    dir.x = -dir.x;

    GameObject* go = engine->ecsManager.Instantiate("Bullet", nullptr, "PlayerBullet");
    go->transform->SetPosition(gameObject->transform->GetLocalPosition());
    engine->ecsManager.GetComponent<RigidBody>(go->GetEntity()).SetLinearVelocity(dir);


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

