#include "Core/engine.hpp"
#include "Core/Maths/solidMaths.hpp"
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

void Megalos_kyvos::Destroy()
{
    for(auto go : CubeBullets)
    {
        engine->ecsManager.DestroyEntity(go->GetEntity());
    }
    CubeBullets.clear();
}


void Megalos_kyvos::Update()
{
    Boss::Update();
    UpdateAttack();

};
void Megalos_kyvos::FixedUpdate()
{



};
void Megalos_kyvos::LateUpdate()
{


};

void Megalos_kyvos::PrimaryAttack()
{
    GameObject* go = engine->ecsManager.Instantiate("CubeBullet",
                                                    nullptr, "cube");
    go->transform->SetPosition(gameObject->transform->GetGlobalPosition());
    CubeBullets.push_back(go);
    StateAttack = EAttack::PRIMARY;

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

void Megalos_kyvos::UpdateAttack()
{
    if(StateAttack == EAttack::NONE)
        return;

    if(StateAttack == EAttack::PRIMARY)
    {
        Vec3 pos = CubeBullets[0]->transform->GetGlobalPosition();
        CubeBullets[0]->transform->SetPosition(Vec3::Nlerp(pos,Target, 0.2 * Time::DeltaTime()));
        Lerp = Vec3::Nlerp(pos, Target, 0.2 * Time::DeltaTime());

        //Lerp = Vec3::Nlerp(gameObject->transform->GetPosition(), Target- gameObject->transform->GetPosition(), 0.2 * Time::DeltaTime());

        CubeBullets[0]->transform->SetPosition(Lerp);
    }
}


