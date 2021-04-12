#pragma once

#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
#include <set>


#include "Build/solidAPI.hpp"
#include "EngineGenerated/meshRenderer.sld.hpp"

namespace Solid SLDNamespace()
{
    class SLDClass() SOLID_API MeshRenderer
    {
    public:
    	MeshRenderer() = default;
    	MeshRenderer(std::shared_ptr<IMesh> _m)
    	{
		    mesh = _m;
		    if(mesh != nullptr)
		    {
		    	materials.resize(0);
		    	materials.resize(mesh->subMeshCount, nullptr);
		    	materialSet.clear();
		    	materialSet.emplace(nullptr);
		    }
    	}
        std::shared_ptr<IMesh> mesh = nullptr;
	    //std::shared_ptr<IShader> shader = nullptr;
	    std::vector<MaterialResource*> materials;
        std::set<MaterialResource*> materialSet;

        MeshRenderer_GENERATED
    };
} //!namespace
File_GENERATED