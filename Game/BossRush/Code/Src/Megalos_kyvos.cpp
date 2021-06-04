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
    StateAttack = EAttack::PRIMARY;
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
    int i = DataBullets[0].Index;
    if(DataBullets[0].Step == EStepAttack::None)
        return;

    DataBullets[0].Ratio += Time::DeltaTime() * SpeedPrimaryAttack;
    float t =  DataBullets[0].Step == EStepAttack::Two ? DataBullets[0].Ratio * 3 : DataBullets[0].Ratio;
    if(DataBullets[0].Step == EStepAttack::One)
    {
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::Two;
            CubeBullets[i]->transform->SetPosition(Vec3::Lerp(gameObject->transform->GetLocalPosition(), Target, 1));
            CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3::Zero, Vec3::One, 1));
            return;
        }
        CubeBullets[i]->transform->SetPosition(Vec3::Lerp(gameObject->transform->GetLocalPosition(), Target, t));
        CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3::Zero, Vec3::One, t));
    }
    else if(DataBullets[0].Step == EStepAttack::Two)
    {

        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::Three;
            CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3::One, Vec3(1,1,5), 1));
            return;
        }
        CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3::One, Vec3(1,1,5), t));
    }
    else if(DataBullets[0].Step == EStepAttack::Three)
    {
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::None;
            return;
        }
        CubeBullets[i]->transform->Rotate( Vec3(0,t/5,0));
    }
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

    else if(StateAttack == EAttack::PRIMARY)
    {
        if(DataBullets.size() <= 0 && CubeBullets.size() <= 0)
        {
            GameObject* go = engine->ecsManager.Instantiate("CubeBullet",
                                                            nullptr, "cube");
            go->transform->SetPosition(gameObject->transform->GetGlobalPosition());
            go->transform->SetScale(Vec3::Zero);
            CubeBullets.push_back(go);
            int i = CubeBullets.size() - 1;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
        }
        PrimaryAttack();
    }
    else if(StateAttack == EAttack::SECONDARY)
    {

    }
    else if(StateAttack == EAttack::TERTIARY)
    {

    }
    else if(StateAttack == EAttack::SPECIAL1)
    {

    }
    else if(StateAttack == EAttack::SPECIAL2)
    {

    }
}


