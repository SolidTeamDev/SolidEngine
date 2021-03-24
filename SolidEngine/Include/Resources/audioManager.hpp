//
// Created by a.galliot on 24/03/2021.
//

#ifndef SOLIDEDITOR_AUDIOMANAGER_HPP
#define SOLIDEDITOR_AUDIOMANAGER_HPP

namespace Solid
{
	class AudioManager
	{
	public:
	//public members

	protected:
	//protected members
		ResourceManager* manager;
		AudioOutput out;
		ALuint currBuff = 0;
		bool loop = false;

	public:
	//public func
	    AudioManager(ResourceManager* _mgr) ;

	    ~AudioManager();
		bool Play(const char* _name);
		bool Play();
		bool Pause();
		bool Stop();
	    void SetLoop(bool _loop);
	    AudioOutput GetOutput();



	};
}
#endif //SOLIDEDITOR_AUDIOMANAGER_HPP
