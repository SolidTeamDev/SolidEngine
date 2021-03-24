//
// Created by a.galliot on 24/03/2021.
//
#include <string>
#include "Core/Maths/Vector/vector3.hpp"
#include "Resources/audioOutput.hpp"

Solid::AudioOutput::AudioOutput()
{
	alGenSources(1, &sourceID);
	alSourcef(sourceID, AL_PITCH, pitch);
	alSourcef(sourceID, AL_GAIN, gain);
	alSource3f(sourceID, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f(sourceID, AL_VELOCITY,velo.x, velo.y, velo.z);
	alSourcei(sourceID, AL_BUFFER, 0);
}

Solid::AudioOutput::~AudioOutput()
{

}

ALuint Solid::AudioOutput::GetOutputID()
{
	return sourceID;
}



void Solid::AudioOutput::setVolume(float _vol)
{
	gain = _vol;
	alSourcef(sourceID, AL_GAIN, _vol);
}

void Solid::AudioOutput::setPitch(float _pitch)
{
	pitch = _pitch;
	alSourcef(sourceID, AL_PITCH, pitch);

}

void Solid::AudioOutput::setMusicPos(Solid::Vec3 _pos)
{
	pos = _pos;
	alSource3f(sourceID, AL_POSITION, pos.x, pos.y, pos.z);

}

void Solid::AudioOutput::setMusicVel(Solid::Vec3 _vel)
{
	velo =_vel;
	alSource3f(sourceID, AL_VELOCITY,velo.x, velo.y, velo.z);
}


