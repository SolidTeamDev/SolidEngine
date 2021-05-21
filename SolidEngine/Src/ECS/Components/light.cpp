#include "ECS/Components/light.hpp"

namespace Solid
{
    std::vector<Light*> Light::lightList;

    void Light::Init()
    {
        lightList.push_back(this);
    }

    void Light::Release()
    {
	    Components::Release();
	    //TODO: Remove light of the list
	    //lightList.erase(lightList.begin()+id);
    }

    std::vector<Light*> Light::GetLightList()
    {
        return lightList;
    }
} //!namespace