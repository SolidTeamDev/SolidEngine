#include "followingObj.hpp"
#include "Core/engine.hpp"
#include "ECS/Components/scriptList.hpp"
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
    Engine* engine = Engine::GetInstance();
    if (effect == nullptr)
    {
        effect = &(engine->ecsManager.GetComponent<ParticleEffect>(gameObject->GetEntity()));
    }
    if (cd == 5000.f)
    {
        ScriptList& scriptList = engine->ecsManager.GetComponent<ScriptList>(following->GetEntity());
        cd = ((PlayerBullet*)(scriptList.GetScript("PlayerBullet")))->lifeTime;
        cd -= 3 * Time::DeltaTime();
        effect->timeGen->maxTime = cd;
        effect->timeGen->minTime = cd;

    }
    cd -= Time::DeltaTime();
    if(following == nullptr || effect == nullptr || cd <= 0.f)
    {
        engine->ecsManager.DestroyEntity(gameObject->GetEntity());
        return;
    }
    effect->spherePosGen->center = following->transform->GetGlobalPosition();

};
void followingObj::FixedUpdate() {


};
void followingObj::LateUpdate() {


};

