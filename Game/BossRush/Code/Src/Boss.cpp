#include "Core/engine.hpp"
#include "Boss.hpp"
using namespace Solid;

Boss::Boss()
{
	
}

Boss::~Boss()
{
	
}

void Boss::Init()
{
    Player = Engine::GetInstance()->ecsManager.FindGameObjectByName("Player");
    if(Player)
        Target = Player->transform->GetPosition();
}

void Boss::Update()
{

};
void Boss::FixedUpdate()
{

};
void Boss::LateUpdate()
{

};

