//
// Created by a.galliot on 24/03/2021.
//

#ifndef SOLIDEDITOR_AUDIOMANAGER_HPP
#define SOLIDEDITOR_AUDIOMANAGER_HPP
#include "Build/solidAPI.hpp"

namespace  Solid
{
	class SOLID_API AudioManager
	{
	public:
	//public members

	protected:
	//protected members
		std::unordered_map<std::string, AudioSource> sources;


	public:
	//public func
	    AudioManager() ;

	    ~AudioManager();
		AudioSource* CreateSource(const char* _sourceName, const char* _soundName = nullptr);
		AudioSource* GetSource(const char *_sourceName);




	};
}
#endif //SOLIDEDITOR_AUDIOMANAGER_HPP
