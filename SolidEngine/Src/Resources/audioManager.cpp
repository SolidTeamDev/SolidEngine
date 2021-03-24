//
// Created by a.galliot on 24/03/2021.
//
#include "Resources/audioManager.hpp"
#include "Resources/resourceType.hpp"
using namespace Solid;

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

bool AudioManager::Play(const char *_name)
{
	AudioResource* audio =manager->GetRawAudioByName(_name);
	if(currBuff == audio->buffer)
		return false;
	currBuff = audio->buffer;
	out.Play(currBuff);

}

bool AudioManager::Play()
{
	alSourcePlay(out.GetOutputID());
	return true;
}

bool AudioManager::Pause()
{
	alSourcePause(out.GetOutputID());
	return true;
}

bool AudioManager::Stop()
{
	alSourceStop(out.GetOutputID());
	return true;
}

void AudioManager::SetLoop(bool _loop)
{
	loop = _loop;
	alSourcei(out.GetOutputID(), AL_LOOPING, loop);
}
