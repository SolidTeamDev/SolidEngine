#include "Core/engine.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "GameManager.hpp"
using namespace Solid;

GameManager::GameManager()
{
	
}

GameManager::~GameManager()
{
	
}

void GameManager::Update() {

	counter -= Time::DeltaTime();

	if(CurrentNumOfEntity < MaxEntityNum && counter < 0)
	{
		if(!PrefabName.empty())
		{
			GameObject* spawnedCube =Engine::GetInstance()->ecsManager.Instantiate(PrefabName, nullptr);
			if(spawnedCube)
			{
				if(Engine::GetInstance()->ecsManager.GotComponent<RigidBody>(spawnedCube->GetEntity()))
				{
					RigidBody& rb = Engine::GetInstance()->ecsManager.GetComponent<RigidBody>(spawnedCube->GetEntity());
					rb.AddImpulse(Vec3(0,ForceProjection,0));
				}
				CurrentNumOfEntity++;
				counter = maxCounter;
			}
		}

	}


};
void GameManager::FixedUpdate() {


};
void GameManager::LateUpdate() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking;
	UI::SetNextWindowPos(ImVec2(10,10));
	UI::SetNextWindowSize(ImVec2(500,500));
	UI::Begin("BAR", nullptr,flags);
	UI::SetWindowFontScale(2);
	UI::Text(" FrameTime : %f ms", Time::DeltaTime());
	UI::Text(" MAX number of entities : %d", MaxEntityNum);
	UI::SameLine();
	if(UI::Button("+ 10"))
	{
		MaxEntityNum += 10;
	}
	UI::Text("number of entities : %d", CurrentNumOfEntity);
	UI::End();

};

