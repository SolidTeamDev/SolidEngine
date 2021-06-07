#include "PlayerBullet.hpp"
#include "Core/engine.hpp"
#include "ECS/Components/scriptList.hpp"
#include "Megalos_kyvos.hpp"

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

    Megalos_kyvos* tmp = (Megalos_kyvos*)engine->ecsManager.GetComponent<ScriptList>(_other->parent->GetEntity()).GetScript("Megalos_kyvos");

    if(!tmp)
        return;

    tmp->BossHealth -= damage;

    //engine->ecsManager.DestroyEntity(gameObject->GetEntity());
}