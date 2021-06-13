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
	    FMOD::System* sys = Engine::GetInstance()->audio.GetSystem();

      //  alSourcef(sourceID, AL_PITCH, pitch);
       // alSourcef(sourceID, AL_GAIN, volume);
       // alSource3f(sourceID, AL_POSITION, 0, 0, 0);
       // alSource3f(sourceID, AL_VELOCITY,velocity.x, velocity.y, velocity.z);

       // alSourcei(sourceID, AL_LOOPING, 0);
       // alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
        audioResource = Engine::GetInstance()->resourceManager.GetRawAudioByName(name.c_str());
        if(audioResource)
        {
	        Engine::GetInstance()->audio.GetSystem()->playSound(audioResource->sound, nullptr, true, &audioChannel);

        }

        isInit = true;
    }

    void AudioSource::SetAudio(AudioResource* _audioResource)
    {
        audioResource = _audioResource;
        name = _audioResource->name;
        Stop();
	    Engine::GetInstance()->audio.GetSystem()->playSound(audioResource->sound, nullptr, true, &audioChannel);


    }

    void AudioSource::SetVolume(float _vol)
    {
        volume = Maths::Clamp<float>(_vol,0,1);
        audioChannel->setVolume(volume);
    }

    void AudioSource::SetPitch(float _pitch)
    {
        pitch = _pitch;

        audioChannel->setPitch( pitch);
    }

    void AudioSource::SetMaxDistance(float _maxDistance)
    {
        maxDistance = _maxDistance;
        float minDistance = 0;
        audioChannel->get3DMinMaxDistance(&minDistance,nullptr);
        audioChannel->set3DMinMaxDistance(minDistance, maxDistance);
    }

    void AudioSource::SetMusicVelocity(const Vec3& _velocity)
    {
        velocity = _velocity;
        FMOD_VECTOR vec {velocity.x,velocity.y,velocity.z};

	    audioChannel->set3DAttributes(nullptr, &vec);
    }

    void AudioSource::SetPosition(const Vec3 &_position)
    {
	    FMOD_VECTOR vec {_position.x,_position.y,_position.z};

	    audioChannel->set3DAttributes(&vec,nullptr);
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
    bool AudioSource::GetIs3D() const
	{
		return is3D;
	}
    void AudioSource::Play()
    {
        if(IsPlaying())
            return;
        audioChannel->setPaused(false);
	    isPlaying = true;
    }

    void AudioSource::Pause()
    {
        if(IsPaused())
            return;
	    audioChannel->setPaused(true);
    }

    void AudioSource::Stop()
    {
        if(!IsPlaying())
            return;
	    audioChannel->setPaused(true);
	    audioChannel->setPosition(0, FMOD_TIMEUNIT_MS);
	    isPlaying = false;
    }

    void AudioSource::SetLoop(bool _loop)
    {
	    if(audioResource == nullptr)
		    return;
    	loop = _loop;
    	FMOD_MODE mode;
    	audioChannel->getMode(&mode);
	    FMOD_MODE mask3d = FMOD_LOOP_NORMAL | FMOD_LOOP_OFF;
	    FMOD_MODE isNotLoop = FMOD_LOOP_OFF & mode;
	    if((loop && isNotLoop != 0) || (!loop && isNotLoop == 0))
	    {
	    	audioChannel->setMode(mode ^ mask3d);
	    }
    }
	void AudioSource::SetIs3D(bool _3D)
	{
    	if(audioResource == nullptr)
		    return;
		is3D = _3D;

		FMOD_MODE cmode;
		audioChannel->getMode(&cmode);

		FMOD_MODE mask3d = FMOD_3D | FMOD_2D;

		if(is3D && ( cmode & FMOD_3D) ==0)
		{
			if(audioChannel->setMode(cmode ^ mask3d) != FMOD_OK)
			{
				Log::Send("MODE m");
			}
		}
		else if(!is3D && ( cmode & FMOD_3D) != 0)
		{
			if(audioChannel->setMode(cmode ^ mask3d) != FMOD_OK)
			{
				Log::Send("MODE");
			}

		}

	}

    bool AudioSource::IsPlaying()
    {
        return isPlaying;
    }

    bool AudioSource::IsPaused()
    {
	    bool isPaused;

	    audioChannel->getPaused(&isPaused);
        return isPaused;
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
		audioChannel->stop();
		audioChannel = nullptr;
	}
} //!namespace