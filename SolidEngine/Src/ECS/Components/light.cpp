#include "ECS/Components/light.hpp"

namespace Solid
{
    std::vector<Light> Light::lightList;

    void Light::Init()
    {
        lightList.push_back(*this);
    }

    void Light::Release()
    {
        //TODO: Remove light of the list
    }
} //!namespace