//
// Created by ryan1 on 28/05/2021.
//

#include "ECS/System/cameraSystem.hpp"
#include "ECS/Components/camera.hpp"

void Solid::CameraSystem::Update()
{
	for (auto entity : entities)
	{
		auto& cam  = ecsManager.GetComponent<Camera>(entity);
		auto& transform = ecsManager.GetComponent<Transform>(entity);
		Vec3 pos =transform.GetGlobalPosition();
		cam.position = pos;
		cam.Euler = transform.GetGlobalEuler();
		Vec3 _front;
		_front.z = Maths::Cos(Maths::DegToRad(cam.Euler.x)) *
		           Maths::Cos(Maths::DegToRad(cam.Euler.y));

		_front.x = Maths::Sin(Maths::DegToRad(cam.Euler.y));

		_front.y = Maths::Sin(Maths::DegToRad(cam.Euler.x)) *
		           Maths::Cos(Maths::DegToRad(cam.Euler.y));

		cam.Front = _front.GetNormalized();
		cam.Right = Vec3::Cross(cam.Front, Vec3::Up).GetNormalized();
		cam.Up = Vec3::Cross(cam.Right, cam.Front).GetNormalized();
	}
}
