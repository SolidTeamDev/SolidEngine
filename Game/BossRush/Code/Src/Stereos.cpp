#include "Stereos.hpp"
#include "Core/engine.hpp"
#include "ECS/Components/scriptList.hpp"
#include "GatheringSolidParticle.hpp"
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
	//PrimaryAttack();
}

void Stereos::Update()
{
    Boss::Update();
    UpdateAttack();
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
	Engine* safeEngine = Engine::GetInstance();
    gatheringParticle = safeEngine->ecsManager.Instantiate("GatherLaserEffect",
                                                    nullptr, "GatheringEffect");
    safeEngine->ecsManager.GetComponent<ScriptList>(gatheringParticle->GetEntity()).Init();
    charging = true;
    cdShoot = 3.f;
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
    if (gatheringParticle != nullptr && StateAttack == EAttack::PRIMARY)
    {
        Engine* safeEngine = Engine::GetInstance();
        ScriptList& scriptList = safeEngine->ecsManager.GetComponent<ScriptList>(gatheringParticle->GetEntity());
        if (scriptList.HasScript("GatheringSolidParticle"))
        {
            GatheringSolidParticle* gsp = (GatheringSolidParticle*)scriptList.GetScript("GatheringSolidParticle");
            if (gsp->autoDestroy > 3.f)
            {
                engine->ecsManager.DestroyEntity(gatheringParticle->GetEntity());
                gatheringParticle = nullptr;
                charging = false;
            }
        }
    }
    if(StateAttack == EAttack::NONE)
    {
        if (ShouldPrimaryAtk)
        {
            PrimaryAttack();
        }
        return;
    }

	if(StateAttack == EAttack::PRIMARY)
	{
	    if (!charging && !shooting)
        {
            cdShoot -= Time::DeltaTime();
            if (cdShoot <= 0.f)
                StateAttack = EAttack::NONE;
        }

	}

}

Quat Stereos::LookAt(Vec3 src, Vec3 dir)
{
    Vec3 fwd = (dir - src).Normalize();

    float dot = Vec3::Dot(Vec3::Forward, fwd);

    if (fabs(dot - (-1.0f)) < 0.000001f)
    {
        return Quat(Vec3::Up.x, Vec3::Up.y, Vec3::Up.z, 3.141f);
    }
    else if (fabs(dot - (1.0f)) < 0.000001f)
    {
        return Quat::Identity;
    }

    float rotAngle = (float)acos(dot);
    Vec3 rotAxis = Vec3::Cross(Vec3::Forward, fwd);
    rotAxis = rotAxis.Normalize();
    return Quat(rotAngle, rotAxis);
}
