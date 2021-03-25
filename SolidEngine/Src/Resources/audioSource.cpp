//
// Created by a.galliot on 24/03/2021.
//
#include <string>
#include "Resources/ressources.hpp"
#include "Resources/resourceMgr.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Resources/audioSource.hpp"
#include "Core/Debug/log.hpp"

using namespace Solid;
Solid::AudioSource::AudioSource(ResourceManager* _mgr)
:manager(_mgr)
{
	alGenSources(1, &sourceID);
	alSourcef(sourceID, AL_PITCH, pitch);
	alSourcef(sourceID, AL_GAIN, gain);
	alSource3f(sourceID, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f(sourceID, AL_VELOCITY,velo.x, velo.y, velo.z);
	alSourcei(sourceID, AL_BUFFER, 0);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
}

Solid::AudioSource::~AudioSource()
{

}

ALuint Solid::AudioSource::GetOutputID()
{
	return sourceID;
}



void Solid::AudioSource::setVolume(float _vol)
{
	gain = _vol;
	alSourcef(sourceID, AL_GAIN, _vol);
}

void Solid::AudioSource::setPitch(float _pitch)
{
	pitch = _pitch;
	alSourcef(sourceID, AL_PITCH, pitch);

}

void Solid::AudioSource::setMusicPos(Solid::Vec3 _pos)
{
	pos = _pos;
	alSource3f(sourceID, AL_POSITION, pos.x, pos.y, pos.z);

}

void Solid::AudioSource::setMusicVel(Solid::Vec3 _vel)
{
	velo =_vel;
	alSource3f(sourceID, AL_VELOCITY,velo.x, velo.y, velo.z);
}


bool AudioSource::Play(const char *_name)
{
	AudioResource* audio =manager->GetRawAudioByName(_name);
	if(audio == nullptr)
	{
		Log::Send(std::string("Sound Named : ") + _name + "NOT FOUND", Log::ELogSeverity::ERROR);
		return false;
	}
	if(currBuff == audio->buffer)
	{
		if(IsPlaying())
			return false;
		else
			Play();
		return true;
	}
	currBuff = audio->buffer;
	alSourceStop(sourceID);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	alSourcei(sourceID, AL_BUFFER, (ALint)currBuff);
	error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	alSourcePlay(sourceID);
	error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	return true;

}

bool AudioSource::Play()
{
	if(IsPlaying())
		return false;
	alSourcePlay(sourceID);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	return true;
}

bool AudioSource::Pause()
{
	if(IsPaused())
		return false;
	alSourcePause(sourceID);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	return true;
}

bool AudioSource::Stop()
{
	if(!IsPlaying())
		return false;
	alSourceStop(sourceID);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
	return true;
}

void AudioSource::SetLoop(bool _loop)
{
	loop = _loop;
	alSourcei(sourceID, AL_LOOPING, loop);
}

bool AudioSource::IsPlaying()
{
	ALint s = AL_STOPPED;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &s);
	if(alGetError() == AL_NO_ERROR && s == AL_PLAYING)
		return true;
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);

	return false;
}

bool AudioSource::IsPaused()
{
	ALint s = AL_STOPPED;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &s);
	if(alGetError() == AL_NO_ERROR && s == AL_PAUSED)
		return true;
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		Log::Send(alGetString(error), Log::ELogSeverity::ERROR);

	return false;
}