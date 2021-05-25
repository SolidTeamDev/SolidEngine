#include "ECS/Components/light.hpp"

namespace Solid
{
	SOLID_API std::vector<LightData> Light::lightList;

    void Light::Init()
    {
	    id = new char ();
        lightList.push_back(LightData{.id = id, .light=this});
    }

    void Light::Release()
    {
	    Components::Release();
	    int i = 0;
	    for(LightData elt : lightList)
	    {
	        if(id == elt.id)
	        {
		        lightList.erase(lightList.begin() + i);
		        break;
	        }
	        ++i;

	    }
	    delete (char*)id;
	    id = nullptr;
		std::cout << lightList.size() << std::endl;
	    //TODO: Remove light of the list
	    //lightList.erase(lightList.begin()+id);
    }

    std::vector<LightData> Light::GetLightList()
    {
        return lightList;
    }

	Light::~Light()
	{

	}

	Light::Light()
	{

	}

	Light::Light(const Light &_light)
	{
		id = _light.id;
	}

	Light &Light::operator=(const Light &_light)
	{
    	id = _light.id;
		return *this;
	}
} //!namespace