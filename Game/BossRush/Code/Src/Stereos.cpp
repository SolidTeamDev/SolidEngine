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
	PrimaryAttack();
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
    if (gatheringParticle != nullptr)
    {
        Engine* safeEngine = Engine::GetInstance();
        ScriptList& scriptList = safeEngine->ecsManager.GetComponent<ScriptList>(gatheringParticle->GetEntity());
        if (scriptList.HasScript("GatheringSolidParticle"))
        {
            GatheringSolidParticle* gsp = (GatheringSolidParticle*)scriptList.GetScript("GatheringSolidParticle");
            if (gsp->autoDestroy > 3.f)
                engine->ecsManager.DestroyEntity(gatheringParticle->GetEntity());
        }
    }
    if(StateAttack == EAttack::NONE)
    {
        if (ShouldPrimaryAtk)
            PrimaryAttack();
        return;
    }

	if(StateAttack == EAttack::PRIMARY)
	{
		cdShoot -= Time::DeltaTime();
		if (cdShoot <= 0.f)
			StateAttack = EAttack::NONE;
	}

}
