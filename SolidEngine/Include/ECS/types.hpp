#pragma once

#include <cstdint>
#include <bitset>
#include <vector>
#include "Refureku/Refureku.h"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/types.sld.hpp"
namespace Solid SLDNamespace()
{
    using Entity = std::uint32_t;
    class GameObject;
    class SOLID_API SLDClass() Components : public rfk::Object
    {

    public:

    	Components() = default;
    	~Components() override = default;
    	Components(const Components& ) = default;
    	virtual void Init() {};
    	GameObject* gameObject = nullptr;
    	Components& operator=(const Components&)
	    {
    		return *this;
	    }
		Components_GENERATED
    };



    const Entity MAX_ENTITIES = 10000;

    using ComponentType = std::uint8_t;
    const ComponentType MAX_COMPONENTS = 32;

    using Signature = std::bitset<MAX_COMPONENTS>;

} //!namespace
File_GENERATED