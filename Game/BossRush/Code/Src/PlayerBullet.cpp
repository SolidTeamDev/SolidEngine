#include "PlayerBullet.hpp"
#include "Boss.hpp"
#include "Core/engine.hpp"
using namespace Solid;

PlayerBullet::PlayerBullet()
{
	
}

PlayerBullet::~PlayerBullet()
{
	
}

void Solid::PlayerBullet::Update()
{
    currentTime += Time::DeltaTime();
    if(currentTime > lifeTime)
        Engine::GetInstance()->ecsManager.DestroyEntity(gameObject->GetEntity());
}

void Solid::PlayerBullet::OnTriggerEnter(Solid::GameObject *_other)
{
    if(_other->tag != "Boss")
        return;

    Engine* engine = Engine::GetInstance();

    engine->ecsManager.GetComponent<Boss*>(_other->GetEntity())->BossHealth -= damage;

    engine->ecsManager.DestroyEntity(gameObject->GetEntity());
}