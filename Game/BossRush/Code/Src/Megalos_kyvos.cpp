#include "Core/engine.hpp"
#include "Core/Maths/Utils/FunctionCurve.hpp"
#include "Core/Maths/solidMaths.hpp"
#include "ECS/Components/boxCollider.hpp"
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
    StateAttack = EAttack::SPECIAL2;
}

void Megalos_kyvos::Destroy()
{


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

    DataBullets[0].Ratio += DataBullets[0].Step == EStepAttack::Two ? Time::DeltaTime() * SpeedPrimaryAttack *5 :
                                                                        Time::DeltaTime() * SpeedPrimaryAttack;
    float t =  DataBullets[0].Ratio;
    BoxCollider* box = &engine->ecsManager.GetComponent<BoxCollider>(CubeBullets[i]->GetEntity());;

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
        CubeBullets[i]->transform->Rotate( Vec3(0,RotationAttackSpeed*Time::DeltaTime(),0));
    }
    if(box != nullptr)
        box->SetSize(CubeBullets[i]->transform->GetLocalScale());
}

void Megalos_kyvos::SecondaryAttack()
{
    int i = DataBullets[0].Index;
    if(DataBullets[0].Step == EStepAttack::None)
        return;

    DataBullets[0].Ratio += Time::DeltaTime() * SpeedSecondaryAttack;
    float t =  DataBullets[0].Ratio;

    BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(CubeBullets[i]->GetEntity());
    BoxCollider* box2 = &engine->ecsManager.GetComponent<BoxCollider>(CubeBullets[i+1]->GetEntity());

    if(DataBullets[0].Step == EStepAttack::One)
    {
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::Two;
            CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3(2,1,1), Vec3(SizeSecondaryAttack,1,1), 1));
            CubeBullets[i+1]->transform->SetScale(Vec3::Lerp(Vec3(1,1,2), Vec3(1,1,SizeSecondaryAttack), 1));
            if(box1 != nullptr)
                box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
            if(box2 != nullptr)
                box2->SetSize(CubeBullets[i+1]->transform->GetLocalScale());
            return;
        }
        CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3(2,1,1), Vec3(SizeSecondaryAttack,1,1), t));
        CubeBullets[i+1]->transform->SetScale(Vec3::Lerp(Vec3(1,1,2), Vec3(1,1,SizeSecondaryAttack), t));
        if(box1 != nullptr)
            box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
        if(box2 != nullptr)
            box2->SetSize(CubeBullets[i+1]->transform->GetLocalScale());

    }
    else if(DataBullets[0].Step == EStepAttack::Two)
    {

        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::None;
            CubeBullets[i]->transform->Rotate( Vec3(0,RotationAttackSpeed*Time::DeltaTime(),0));
            CubeBullets[i+1]->transform->Rotate( Vec3(0,RotationAttackSpeed*Time::DeltaTime(),0));

            return;
        }
        CubeBullets[i]->transform->Rotate( Vec3(0,RotationAttackSpeed*Time::DeltaTime(),0));
        CubeBullets[i+1]->transform->Rotate( Vec3(0,RotationAttackSpeed*Time::DeltaTime(),0));

    }

}

void Megalos_kyvos::TertiaryAttack()
{
    int i = DataBullets[0].Index;
    if(DataBullets[0].Step == EStepAttack::None)
        return;

    DataBullets[0].Ratio += Time::DeltaTime() * SpeedSecondaryAttack;
    float t =  DataBullets[0].Ratio;

    if(DataBullets[0].Step == EStepAttack::One)
    {
        Vec3 pos = Vec3(Target.x, Target.y + 5, Target.z);
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::Two;
            CubeBullets[i]->transform->SetPosition(Vec3::Lerp(gameObject->transform->GetLocalPosition(), pos, 1));
            return;
        }
        CubeBullets[i]->transform->SetPosition(Vec3::Lerp(gameObject->transform->GetLocalPosition(), pos, t));
    }

    else if(DataBullets[0].Step == EStepAttack::Two)
    {
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::None;
            for(auto go : CubeBullets[i]->childs)
            {
                go->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(1,10,1), 1));
                BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(go->GetEntity());
                if(box1 != nullptr)
                    box1->SetSize(CubeBullets[i]->transform->GetLocalScale());

            }
            return;
        }
        for(auto go : CubeBullets[i]->childs)
        {
            go->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(1,10,1), t));
            BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(go->GetEntity());
            if(box1 != nullptr)
                box1->SetSize(CubeBullets[i]->transform->GetLocalScale());

        }
    }
}

void Megalos_kyvos::Special1Attack()
{
    int i = DataBullets[0].Index;
    if(DataBullets[0].Step == EStepAttack::None)
        return;

    DataBullets[0].Ratio += Time::DeltaTime() * SpeedSecondaryAttack;
    float t =  DataBullets[0].Ratio;
    if(DataBullets[0].Step == EStepAttack::One)
    {
        if (t > 1)
        {
            DataBullets[0].Ratio = 0;
            DataBullets[0].Step = EStepAttack::None;


            for(auto go : CubeBullets[i]->childs)
            {
                go->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(10,1,1), t));
                go->transform->Rotate( Vec3(0,10*Time::DeltaTime(),0));

                BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(go->GetEntity());
                if(box1 != nullptr)
                    box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
            }

            return;
        }
        CubeBullets[i]->transform->Rotate( Vec3(0,10*Time::DeltaTime(),0));

        for(auto go : CubeBullets[i]->childs)
        {
            go->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(10,1,1), t));

            BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(go->GetEntity());
            if(box1 != nullptr)
                box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
        }

    }
}

void Megalos_kyvos::Special2Attack()
{


    for(auto Data : DataBullets)
    {
        int i = Data.Index;
        if(Data.Step == EStepAttack::None)
            continue;

        Data.Ratio += Time::DeltaTime() * SpeedSecondaryAttack;
        if(Data.Step == EStepAttack::One)
        {
            Log::Send("Size " + std::to_string(DataBullets.size()) + ":" + std::to_string(CubeBullets.size()));
            if(Data.Ratio >= 1)
            {
                Data.Step = EStepAttack::None;
                CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(1,10,1),1));
                BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(CubeBullets[i]->GetEntity());
                if(box1 != nullptr)
                    box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
            }
            CubeBullets[i]->transform->SetScale(Vec3::Lerp(Vec3(1,1,1), Vec3(1,10,1), Data.Ratio));
            BoxCollider* box1 = &engine->ecsManager.GetComponent<BoxCollider>(CubeBullets[i]->GetEntity());
            if(box1 != nullptr)
                box1->SetSize(CubeBullets[i]->transform->GetLocalScale());
        }
    }
}

void Megalos_kyvos::ChooseAttack()
{

}
void Megalos_kyvos::CreateBulletsCube(std::string namePrefab, Vec3 pos, Vec3 scale)
{
    GameObject* go = engine->ecsManager.Instantiate(namePrefab,nullptr, namePrefab + std::to_string(CubeBullets.size()));
    go->transform->SetPosition(pos);
    go->transform->SetScale(scale);
    CubeBullets.push_back(go);
}
void Megalos_kyvos::UpdateAttack()
{
    if(StateAttack == EAttack::NONE)
        return;

    else if(StateAttack == EAttack::PRIMARY)
    {
        if(DataBullets.size() <= 0 && CubeBullets.size() <= 0)
        {
            CreateBulletsCube("CubeBullet", gameObject->transform->GetLocalPosition(), Vec3::Zero);
            int i = CubeBullets.size() - 1;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
        }
        PrimaryAttack();
    }
    else if(StateAttack == EAttack::SECONDARY)
    {
        if(DataBullets.size() <= 0 && CubeBullets.size() <= 0)
        {
            Vec3 pos = Vec3(gameObject->transform->GetLocalPosition().x,1,
                            gameObject->transform->GetLocalPosition().z);
            CreateBulletsCube("CubeBullet", pos, Vec3(2,1,1));
            CreateBulletsCube("CubeBullet", pos, Vec3(1,1,2));

            int i = CubeBullets.size() - 2;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
        }
        SecondaryAttack();
    }
    else if(StateAttack == EAttack::TERTIARY)
    {
        if(DataBullets.size() <= 0 && CubeBullets.size() <= 0)
        {

            CreateBulletsCube("MultiPillar", gameObject->transform->GetLocalPosition(), Vec3(0.5));
            int i = CubeBullets.size() - 1;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
        }
        TertiaryAttack();
    }
    else if(StateAttack == EAttack::SPECIAL1)
    {
        if(DataBullets.size() <= 0 && CubeBullets.size() <= 0)
        {

            CreateBulletsCube("BulletSA1", Target, Vec3(1));
            int i = CubeBullets.size() - 1;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
        }
        Special1Attack();
    }
    else if(StateAttack == EAttack::SPECIAL2)
    {
        if(CurrTimeSpe >= TimeSpecialAttack && CurrNumber < NumberSpawnCubeS2)
        {
            Vec3 pos = Vec3(Player->transform->GetLocalPosition().x,
                            Player->transform->GetLocalPosition().y+5,
                            Player->transform->GetLocalPosition().z);
            CreateBulletsCube("CubeBullet",pos , Vec3(1));
            int i = CubeBullets.size() - 1;
            DataBullets.push_back({i, 0,Vec3::Zero, EStepAttack::One});
            CurrTimeSpe = 0;
            CurrNumber++;
        }
        Special2Attack();
        CurrTimeSpe+=Time::DeltaTime();
    }
}


