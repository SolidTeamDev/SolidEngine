#include "followingObj.hpp"
#include "Core/engine.hpp"
#include "ECS/Components/scriptList.hpp"
#include <iostream>
#include "PlayerBullet.hpp"
using namespace Solid;

followingObj::followingObj()
{
	
}

followingObj::~followingObj()
{
	
}

void followingObj::Init()
{
}

void followingObj::Update()
{
    /*Engine* engine = Engine::GetInstance();
    if (effect == nullptr)
    {
        if (engine->ecsManager.GotComponent<ParticleEffect>(gameObject->GetEntity()))
            effect = &(engine->ecsManager.GetComponent<ParticleEffect>(gameObject->GetEntity()));
    }
    else
    {
        effect->timeGen->maxTime = 0.3f;
        effect->timeGen->minTime = 0.5f;
        effect->eulerUpdater->globalAcceleration = dir + dir;
    }
    if (rb == nullptr)
    {
        if (engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
            rb = &(engine->ecsManager.GetComponent<RigidBody>(gameObject->GetEntity()));
    }
    else
        dir = rb->GetLinearVelocity();*/

};
void followingObj::FixedUpdate() {


};
void followingObj::LateUpdate() {


};

