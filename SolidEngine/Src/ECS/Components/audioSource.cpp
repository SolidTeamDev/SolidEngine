#include "ECS/Components/audioSource.hpp"

#include <string>
#include "Resources/ressources.hpp"
#include "Resources/resourceMgr.hpp"

#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/solidMaths.hpp"
#include "Core/Debug/log.hpp"
#include "Core/engine.hpp"
namespace Solid
{
    AudioSource::~AudioSource()
    {

    }

    void AudioSource::Init()
    {
        alGenSources(1, &sourceID);
        alSourcef(sourceID, AL_PITCH, pitch);
        alSourcef(sourceID, AL_GAIN, volume);
        alSource3f(sourceID, AL_POSITION, 0, 0, 0);
        alSource3f(sourceID, AL_VELOCITY,velocity.x, velocity.y, velocity.z);

        alSourcei(sourceID, AL_LOOPING, 0);
        alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
        audioResource = Engine::GetInstance()->resourceManager.GetRawAudioByName(name.c_str());
        if(audioResource != nullptr)
	        alSourcei(sourceID, AL_BUFFER, audioResource->buffer);
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
        {
            const char * str =alGetString(error);
            if(str != nullptr)
                Log::Send(str, Log::ELogSeverity::ERROR);
        }


        isInit = true;
    }

    void AudioSource::SetAudio(AudioResource* _audioResource)
    {
        audioResource = _audioResource;
        name = _audioResource->name;
        Stop();
        alSourcei(sourceID, AL_BUFFER, audioResource->buffer);
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
            Log::Send(alGetString(error), Log::ELogSeverity::ERROR);

    }

    void AudioSource::SetVolume(float _vol)
    {
        volume = Maths::Clamp<float>(_vol,0,1);
        alSourcef(sourceID, AL_GAIN, volume);
    }

    void AudioSource::SetPitch(float _pitch)
    {
        pitch = _pitch < 0 ? 0 : _pitch;
        alSourcef(sourceID, AL_PITCH, pitch);
    }

    void AudioSource::SetMaxDistance(float _maxDistance)
    {
        maxDistance = _maxDistance;
        alSourcef(sourceID,AL_MAX_DISTANCE,_maxDistance);
    }

    void AudioSource::SetMusicVelocity(const Vec3& _velocity)
    {
        velocity = _velocity;
        alSource3f(sourceID, AL_VELOCITY,velocity.x, velocity.y, velocity.z);
    }

    void AudioSource::SetPosition(const Vec3 &_position)
    {
        alSource3f(sourceID, AL_POSITION, _position.x, _position.y, _position.z);
    }

    std::string AudioSource::GetName() const
    {
        return name;
    }

    float AudioSource::GetVolume() const
    {
        return volume;
    }

    float AudioSource::GetPitch() const
    {
        return pitch;
    }

    float AudioSource::GetMaxDistance() const
    {
        return maxDistance;
    }

    Vec3 AudioSource::GetMusicVelocity() const
    {
        return velocity;
    }

    void AudioSource::Play()
    {
        if(IsPlaying())
            return;
        alSourcePlay(sourceID);
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
            Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
    }

    void AudioSource::Pause()
    {
        if(IsPaused())
            return;
        alSourcePause(sourceID);
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
            Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
    }

    void AudioSource::Stop()
    {
        if(!IsPlaying())
            return;
        alSourceStop(sourceID);
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
            Log::Send(alGetString(error), Log::ELogSeverity::ERROR);
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

    bool AudioSource::IsLooping()
    {
        return loop;
    }

	void AudioSource::Release()
	{
		if(!isInit)
			return;
		Components::Release();
		isInit = false;
		audioResource = nullptr;
		alDeleteSources(1, &sourceID);
	}
} //!namespace