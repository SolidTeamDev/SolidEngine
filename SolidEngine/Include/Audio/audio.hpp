#pragma once
#include <unordered_map>
#include "fmod.hpp"

namespace Solid
{
    class Audio
    {
	    FMOD::System *system = nullptr;
	    std::unordered_map<std::string, FMOD::ChannelGroup *> channelGroups;
    public:

    	Audio();
        ~Audio();

	    FMOD::System * GetSystem();
	    FMOD::ChannelGroup * AddChannelGroup(const char* name);
	    FMOD::ChannelGroup * GetChannelGroup(const char* name);
	    void RemoveChannelGroup(const char* name);
    };
} //!namespace
