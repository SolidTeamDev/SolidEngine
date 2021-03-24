//
// Created by a.galliot on 24/03/2021.
//

#ifndef SOLIDEDITOR_AUDIOOUTPUT_HPP
#define SOLIDEDITOR_AUDIOOUTPUT_HPP
#include "AL/alc.h"
#include "AL/al.h"
#include "AL/alext.h"

namespace Solid
{
	class AudioOutput
	{
	public:
	//public members

		float pitch = 1.f;
		float gain = 1.f;
		Vec3 pos = {0, 0, 0};
		Vec3 velo = {0,0,0};

	protected:
	//protected members
		ALuint sourceID = 0;
	
	public:
	//public func
	    AudioOutput();
	    ~AudioOutput() ;
	    ALuint GetOutputID();
		void setVolume(float _vol);
		void setPitch(float _pitch);
		void setMusicPos(Vec3 _pos);
		void setMusicVel(Vec3 _vel);

	};
}
#endif //SOLIDEDITOR_AUDIOOUTPUT_HPP
