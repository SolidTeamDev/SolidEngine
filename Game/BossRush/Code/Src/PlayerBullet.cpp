#include "PlayerBullet.hpp"

#include "Core/engine.hpp"
using namespace Solid;

PlayerBullet::PlayerBullet()
{
	
}

PlayerBullet::~PlayerBullet()
{
	
}

void Solid::PlayerBullet::Destroy()
{
    Engine::GetInstance()->ecsManager.DestroyEntity(gameObject->GetEntity());
}