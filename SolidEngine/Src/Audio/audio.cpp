#include "Audio/audio.hpp"

#include "Core/Debug/log.hpp"

#include "AL/al.h"
#include "AL/alc.h"

namespace Solid
{
    Audio::Audio()
    {
        ALCdevice* device = alcOpenDevice(nullptr);
        ALCcontext* context = nullptr;
        if(device)
        {
            context = alcCreateContext(device, nullptr);
            alcMakeContextCurrent(context);
        }

        const char* name = nullptr;

        if(alcIsExtensionPresent(device,"ALC_ENUMERATE_ALL_EXT"))
        {
            name = alcGetString(device,ALC_ALL_DEVICES_SPECIFIER);
        }

        if(!name || alcGetError(device) != AL_NO_ERROR)
        {
            name = alcGetString(device,ALC_DEVICE_SPECIFIER);
        }

        Log::Send(name);


        alGetError();
    }

    Audio::~Audio()
    {
        ALCcontext* context = alcGetCurrentContext();
        ALCdevice* device = alcGetContextsDevice(context);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
} //!namespace