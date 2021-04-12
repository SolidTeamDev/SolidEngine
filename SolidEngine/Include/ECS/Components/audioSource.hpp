#pragma once


#include "Core/Maths/Vector/vector3.hpp"
#include "Resources/resourceType.hpp"

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>

#include "Build/solidAPI.hpp"
#include "EngineGenerated/audioSource.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() AudioSource
    {
    private:
        bool isInit = false;

        std::string name = "None";

        ALuint sourceID = 0;
        AudioResource* audioResource = nullptr;
        bool loop = false;
        float pitch = 1.f;
        float volume = 1.f;
        float maxDistance = 500;
        Vec3 velocity = {0,0,0};

    public:
        //public func
        AudioSource() = default;
        ~AudioSource();
        void Init();
        void SetAudio(AudioResource* _audioResource);
        void SetLoop(bool _loop);
        void SetVolume(float _vol);
        void SetPitch(float _pitch);
        void SetMaxDistance(float _maxDistance);
        void SetMusicVelocity(const Vec3& _velocity);
        void SetPosition(const Vec3& _position);

        std::string GetName() const;
        float GetVolume() const;
        float GetPitch() const;
        float GetMaxDistance() const;
        Vec3  GetMusicVelocity() const;

        void Play();
        void Pause();
        void Stop();
        bool IsPlaying();
        bool IsPaused();
        bool IsLooping();
		AudioSource_GENERATED
    };
} //!namespace

File_GENERATED