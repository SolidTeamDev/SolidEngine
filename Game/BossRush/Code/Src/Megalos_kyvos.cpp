#include "Core/engine.hpp"
#include "Core/Maths/Utils/FunctionCurve.hpp"
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
    go->transform->SetPosition(gameObject->transform->GetPosition());
    CubeBullets.push_back(go);
    int i = CubeBullets.size() - 1;
    DataBullets.push_back({i, 0,Vec3::Zero});
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
        int i = DataBullets[0].Index;
        DataBullets[0].Ratio += Time::DeltaTime() * SpeedPrimaryAttack;
        if(DataBullets[0].Ratio > 1)
        {
            DataBullets[0].Ratio = 1;
            return;
        }
        float t = FunctionCurve::easeInOutBack(DataBullets[0].Ratio);

        CubeBullets[i]->transform->SetPosition(Vec3::Lerp(gameObject->transform->GetPosition(), Target, t));
    }
}


