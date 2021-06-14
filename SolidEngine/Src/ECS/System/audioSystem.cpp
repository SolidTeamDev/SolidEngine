#include "Core/engine.hpp"
#include "ECS/System/audioSystem.hpp"
#include "ECS/Components/rigidBody.hpp"

#include "ECS/Components/audioSource.hpp"

namespace Solid
{
    void AudioSystem::Update(Camera& _camera)
    {
        //TODO: Update with audio system -> find better solution
        const Vec3 cameraPos = _camera.position;
	    FMOD_VECTOR velocity= {0,0,0};
	    FMOD_VECTOR pos= {cameraPos.x,cameraPos.y,cameraPos.z};
	    FMOD_VECTOR front= {_camera.Front.x,_camera.Front.y,_camera.Front.z};
	    FMOD_VECTOR up= {_camera.Up.x,_camera.Up.y,_camera.Up.z};

        Engine::GetInstance()->audio.GetSystem()->update();
        if(_camera.gameObject != nullptr)
        {
            if(Engine::GetInstance()->ecsManager.GotComponent<RigidBody>(_camera.gameObject->GetEntity()))
            {
	            Vec3 vel =Engine::GetInstance()->ecsManager.GetComponent<RigidBody>(_camera.gameObject->GetEntity()).GetLinearVelocity();
	            velocity = {vel.x,vel.y,vel.z};
            }
        }
	    Engine::GetInstance()->audio.GetSystem()->set3DListenerAttributes(0, &pos, &velocity,&front, &up);
        for (auto entity : entities)
        {
            auto& audioSource  = ecsManager.GetComponent<AudioSource>(entity);
            auto& transform = ecsManager.GetComponent<Transform>(entity);

            audioSource.SetPosition(transform.GetLocalPosition());
        }
    }
} //!namespace

