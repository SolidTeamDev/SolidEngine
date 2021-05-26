#include "Core/engine.hpp"
#include "Boss1.hpp"
using namespace Solid;

void Boss1::Init()
{
    Pv = 10.f;
}

void Boss1::Update()
{


};
void Boss1::FixedUpdate() {


};
void Boss1::LateUpdate() {


};

void Boss1::UpdateTarget()
{
    if(Player == nullptr)
        Player = Engine::GetInstance()->ecsManager.GetGameObjectFromEntity(PlayerEntityId);
    Vec3 _target = Player->transform->GetPosition();
    target = _target;
}

