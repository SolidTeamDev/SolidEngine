#pragma once

#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
#include <set>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/meshRenderer.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass()  MeshRenderer : public Components
    {
    private:

	    std::shared_ptr<IMesh> mesh = nullptr;

	    SLDField()
        vectorStr MatNames;
	    SLDField()
	    String MeshName;
	    std::vector<MaterialResource*> materials;
	    std::set<MaterialResource*> materialSet;

    public:
	    MeshRenderer();
	    MeshRenderer(std::shared_ptr<IMesh> _m);

	    std::shared_ptr<IMesh> GetMesh() const;

	    void SetMesh(const std::shared_ptr<IMesh> _mesh);

	    const std::vector<MaterialResource *>& GetMaterials() const ;
	    void SetMaterials(const std::vector<MaterialResource *>&);

	    std::set<MaterialResource *>& GetMaterialSet() ;

	    void SetMaterialAt(int _index, MaterialResource* _mat);

	    void SetMaterialSet(const std::set<MaterialResource *> &_materialSet);

	    void Init() override;

        MeshRenderer_GENERATED
    };


} //!namespace
File_GENERATED