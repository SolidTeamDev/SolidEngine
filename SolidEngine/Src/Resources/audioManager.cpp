//
// Created by a.galliot on 24/03/2021.
//
#include "Core/engine.hpp"
#include "Resources/ressources.hpp"
#include "Resources/resourceMgr.hpp"
#include "Resources/audioSource.hpp"
#include "Resources/audioManager.hpp"
#include "Core/Debug/log.hpp"

using namespace Solid;

AudioManager::AudioManager()
{
	static ALCdevice* cDevice = alcOpenDevice(nullptr);
	if (!cDevice)
		ThrowError("Couldn't get the sound device", ESolidErrorCode::S_INIT_ERROR);

	static ALCcontext* cContext = alcCreateContext(cDevice, nullptr);
	if (!cContext)
		ThrowError("Couldn't create a context", ESolidErrorCode::S_INIT_ERROR);

	if (!alcMakeContextCurrent(cContext))
		ThrowError("Couldn't make the context current", ESolidErrorCode::S_INIT_ERROR);

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(cDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(cDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(cDevice) != AL_NO_ERROR)
		name = alcGetString(cDevice, ALC_DEVICE_SPECIFIER);
	Log::Send(name, Log::ELogSeverity::INFO);
}

AudioManager::~AudioManager()
{

}

AudioSource *AudioManager::CreateSource(const char *_sourceName, const char *_soundName)
{
	if(sources.find(_sourceName) != sources.end())
		return nullptr;
	auto source =sources.emplace(_sourceName, AudioSource(&Engine::GetInstance()->resourceManager));
	if(_soundName != nullptr)
		source.first->second.Play(_soundName);
	return &source.first->second;
}

AudioSource *AudioManager::GetSource(const char *_sourceName)
{
	if(sources.find(_sourceName) == sources.end())
		return nullptr;
	return &(sources.at(_sourceName));
}


