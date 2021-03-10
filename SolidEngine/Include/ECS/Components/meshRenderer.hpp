#pragma once

#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
namespace Solid
{
    struct SOLID_API MeshRenderer
    {
        std::shared_ptr<IMesh> mesh = nullptr;
	    std::shared_ptr<IShader> shader = nullptr;
    };
} //!namespace