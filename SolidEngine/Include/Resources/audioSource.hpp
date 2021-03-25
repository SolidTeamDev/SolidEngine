//
// Created by a.galliot on 24/03/2021.
//

#ifndef SOLIDEDITOR_AUDIOSOURCE_HPP
#define SOLIDEDITOR_AUDIOSOURCE_HPP
#include "AL/alc.h"
#include "AL/al.h"
#include "AL/alext.h"
namespace Solid
{
	class SOLID_API AudioSource
	{
	public:
	//public members



	protected:
	//protected members
		ALuint sourceID = 0;
		ALuint currBuff = 0;
		bool loop = false;
		float pitch = 1.f;
		float gain = 1.f;
		Vec3 pos = {0, 0, 0};
		Vec3 velo = {0,0,0};
		ResourceManager* manager;
	
	public:
	//public func
	    AudioSource(ResourceManager* _mgr);
	    ~AudioSource() ;
	    ALuint GetOutputID();
		void setVolume(float _vol);
		void setPitch(float _pitch);
		void setMusicPos(Vec3 _pos);
		void setMusicVel(Vec3 _vel);
		bool Play(const char* _name);
		bool Play();
		bool Pause();
		bool Stop();
		void SetLoop(bool _loop);
		bool IsPlaying();
		bool IsPaused();

	};
}
#endif //SOLIDEDITOR_AUDIOSOURCE_HPP
