#pragma once

#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
namespace Solid
{
    struct SOLID_API MeshRenderer
    {
    	MeshRenderer() = default;
    	MeshRenderer(std::shared_ptr<IMesh> _m)
    	{
		    mesh = _m;
		    if(mesh != nullptr)
		    {
		    	materials.resize(0);
		    	materials.resize(mesh->subMeshCount, nullptr);
		    }
    	}
        std::shared_ptr<IMesh> mesh = nullptr;
	    //std::shared_ptr<IShader> shader = nullptr;
	    std::vector<MaterialResource*> materials;
    };
} //!namespace