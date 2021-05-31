#include "Core/engine.hpp"
#include "Megalos_kyvos.hpp"
using namespace Solid;

Megalos_kyvos::Megalos_kyvos()
{
	
}

Megalos_kyvos::~Megalos_kyvos()
{
	
}

void Megalos_kyvos::Init()
{
    Boss::Init();
    PrimaryAttack();

}

void Megalos_kyvos::Update()
{


};
void Megalos_kyvos::FixedUpdate()
{



};
void Megalos_kyvos::LateUpdate()
{


};

void Megalos_kyvos::PrimaryAttack()
{
    Log::Send("pass", Log::ELogSeverity::WARNING);
    GameObject* go = engine->ecsManager.Instantiate("CubeBullet",
                                                    gameObject, "cube");
    go->ReParentCurrent(engine->ecsManager.GetWorld());
    CubeBullets.push_back(go);
}

void Megalos_kyvos::SecondaryAttack()
{

}

void Megalos_kyvos::TertiaryAttack()
{

}

void Megalos_kyvos::Special1Attack()
{

}

void Megalos_kyvos::Special2Attack()
{

}

void Megalos_kyvos::ChooseAttack()
{

}

