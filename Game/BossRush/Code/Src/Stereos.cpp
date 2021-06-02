#include "Stereos.hpp"
#include "Core/engine.hpp"
using namespace Solid;

Stereos::Stereos()
{
	
}

Stereos::~Stereos()
{
	
}

void Stereos::Init()
{
	Boss::Init();
}

void Stereos::Update() {


};
void Stereos::FixedUpdate() {


};
void Stereos::LateUpdate() {


};

void Stereos::Destroy()
{

}

void Stereos::PrimaryAttack()
{
	StateAttack = EAttack::PRIMARY;

}

void Stereos::SecondaryAttack()
{

}

void Stereos::TertiaryAttack()
{

}

void Stereos::Special1Attack()
{

}

void Stereos::Special2Attack()
{

}

void Stereos::ChooseAttack()
{

}

void Stereos::UpdateAttack()
{
	if(StateAttack == EAttack::NONE)
		return;

	if(StateAttack == EAttack::PRIMARY)
	{
		cdShoot -= Time::DeltaTime();
		if (cdShoot <= 0.f)
			StateAttack = EAttack::NONE;
	}

}
