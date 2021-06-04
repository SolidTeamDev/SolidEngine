#include "Core/engine.hpp"
#include <cstdlib>
#include "Rotate.hpp"
using namespace Solid;

Rotate::Rotate()
{
	
}

Rotate::~Rotate()
{
	
}

void Rotate::Update()
{
    if(!RandomAngle)
        gameObject->transform->Rotate(Angle*SpeedRotation*Time::DeltaTime());
    else
    {
        gameObject->transform->Rotate(Angle * SpeedRotation * Time::DeltaTime());
    }
};
void Rotate::FixedUpdate() {


};
void Rotate::LateUpdate() {


};

