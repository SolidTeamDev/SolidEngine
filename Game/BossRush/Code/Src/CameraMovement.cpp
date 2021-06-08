#include "Core/engine.hpp"
#include "ECS/Components/camera.hpp"
#include "CameraMovement.hpp"
using namespace Solid;

CameraMovement::CameraMovement()
{
	
}

CameraMovement::~CameraMovement()
{
	
}
void CameraMovement::Init()
{
    engine = Engine::GetInstance();

    camera1 = engine->ecsManager.FindGameObjectByName("camera1");
    camera2 = engine->ecsManager.FindGameObjectByName("camera2");
    engine->SetActiveCamera(&engine->ecsManager.GetComponent<Camera>(camera1->GetEntity()));
}

void CameraMovement::Update()
{
        t += Time::DeltaTime() * CamSpeed;

        if(!IsSecond)
        {
            camera1->transform->SetPosition(Vec3::Lerp(Vec3(-10,9,27), Vec3(10,9,27),t));
            if(t >= 1)
            {
                IsSecond = true;
                engine->SetActiveCamera(&engine->ecsManager.GetComponent<Camera>(camera2->GetEntity()));
                t = 0;
            }
        }
        else if(IsSecond && t <= 1)
            camera2->transform->SetPosition(Vec3::Lerp(Vec3(-10,9,6), Vec3(10,9,6),t));


};


