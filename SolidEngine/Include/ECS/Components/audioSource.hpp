#pragma once


#include "Core/Maths/Vector/vector3.hpp"
#include "Resources/resourceType.hpp"

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>
#include <string>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/audioSource.sld.hpp"


namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() AudioSource : public Components
    {
    private:
        bool isInit = false;

        SLDField()
        String name = "None";

        ALuint sourceID = 0;
        AudioResource* audioResource = nullptr;

        SLDField()
        bool loop = false;
	    SLDField()
	    float pitch = 1.f;
	    SLDField()
        float volume = 1.f;
	    SLDField()
        float maxDistance = 500;
	    SLDField()
        Vec3 velocity = {0,0,0};

    public:
        //public func
        AudioSource() = default;
        ~AudioSource();
        SLDMethod()
        void Init();
	    SLDMethod()
        void SetAudio(AudioResource* _audioResource);
	    SLDMethod()
        void SetLoop(bool _loop);
	    SLDMethod()
	    void SetVolume(float _vol);
	    SLDMethod()
	    void SetPitch(float _pitch);
	    SLDMethod()
	    void SetMaxDistance(float _maxDistance);
	    SLDMethod()
	    void SetMusicVelocity(const Vec3& _velocity);
	    SLDMethod()
	    void SetPosition(const Vec3& _position);

	    SLDMethod()
	    std::string GetName() const;
	    SLDMethod()
	    float GetVolume() const;
	    SLDMethod()
	    float GetPitch() const;
	    SLDMethod()
	    float GetMaxDistance() const;
	    SLDMethod()
	    Vec3  GetMusicVelocity() const;

	    SLDMethod()
	    void Play();
	    SLDMethod()
	    void Pause();
	    SLDMethod()
	    void Stop();
	    SLDMethod()
	    bool IsPlaying();
	    SLDMethod()
	    bool IsPaused();
	    SLDMethod()
	    bool IsLooping();
		AudioSource_GENERATED
    };
} //!namespace

File_GENERATED