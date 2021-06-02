#include "ECS/System/audioSystem.hpp"

#include "ECS/Components/audioSource.hpp"

namespace Solid
{
    void AudioSystem::Update(Camera& _camera)
    {
        //TODO: Update with audio system -> find better solution
        const Vec3 cameraPos = _camera.position;
        alListener3f(AL_POSITION,cameraPos.x,cameraPos.y,cameraPos.z);
        for (auto entity : entities)
        {
            auto& audioSource  = ecsManager.GetComponent<AudioSource>(entity);
            auto& transform = ecsManager.GetComponent<Transform>(entity);

            audioSource.SetPosition(transform.GetLocalPosition());
        }
    }
} //!namespace

