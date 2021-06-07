#include "GameManager.hpp"
#include "ECS/Components/audioSource.hpp"

using namespace Solid;

GameManager::GameManager()
{
	
}

GameManager::~GameManager()
{
	
}

void GameManager::Init()
{
    engine = Engine::GetInstance();

    AudioSource& mainTheme = engine->ecsManager.GetComponent<AudioSource>(gameObject->GetEntity());

    mainTheme.SetAudio(engine->resourceManager.GetRawAudioByName("ActionCrave.wav"));

    mainTheme.Play();
}

void Solid::GameManager::Destroy()
{

}

void GameManager::Update()
{

}

