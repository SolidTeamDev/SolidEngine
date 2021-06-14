#include "Audio/audio.hpp"

#include "Core/Debug/log.hpp"


namespace Solid
{
    Audio::Audio()
    {
	    FMOD_RESULT result;


	    if(!system)
		    result = FMOD::System_Create(&system);
	    //if (result != FMOD_OK)
		//    return ;
	    result = system->init(512, FMOD_INIT_NORMAL |FMOD_INIT_3D_RIGHTHANDED, nullptr);
	    FMOD::ChannelGroup *channelGroup = nullptr;
	    result = system->createChannelGroup("Master", &channelGroup);
	    channelGroups.emplace("Master", channelGroup);


    }

    Audio::~Audio()
    {
    	for(auto& elt : channelGroups)
    	{
    	    elt.second->release();
    	}
	    system->release();
    }

	FMOD::System *Audio::GetSystem()
	{
		return system;
	}

	FMOD::ChannelGroup *Audio::AddChannelGroup(const char *name)
	{
		FMOD_RESULT result;
		FMOD::ChannelGroup *channelGroup = nullptr;
		result = system->createChannelGroup(name, &channelGroup);
		channelGroups.emplace(name, channelGroup);
		return channelGroup;
	}

	FMOD::ChannelGroup *Audio::GetChannelGroup(const char *name)
	{
		try
		{
			return channelGroups.find(name)->second;
		}
		catch (std::exception& e)
		{
			return nullptr;
		}

	}

	void Audio::RemoveChannelGroup(const char *name)
	{
		channelGroups.erase(name);
	}
} //!namespace