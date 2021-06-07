//
// Created by ryan1 on 02/06/2021.
//
#include "Core/engine.hpp"
#include "ECS/System/animationSystem.hpp"
#include "ECS/Components/animation.hpp"

namespace Solid
{
	void AnimationSystem::Update()
	{
		for (auto entity : entities)
		{
			auto& anim  = ecsManager.GetComponent<Animation>(entity);
			anim.UpdateAnim(Time::DeltaTime());
		}
	}
    void AnimationSystem::DebugUpdate(Camera& _camera)
    {
            for (auto entity : entities)
            {
                auto &anim = ecsManager.GetComponent<Animation>(entity);
                std::vector<Vec3> points;
                std::vector<uint> indices;
                anim.DrawSkeleton(points, indices);
                Engine::GetInstance()->renderer->DrawLines(_camera, points, indices);
            }
    }

} //!namespace

