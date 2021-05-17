//
// Created by ryan1 on 13/04/2021.
//

#include "ECS/Components/meshRenderer.hpp"
#include "Core/engine.hpp"

using namespace Solid;

Solid::MeshRenderer::MeshRenderer(std::shared_ptr<IMesh> _m)
{
	mesh = _m;
	if(mesh != nullptr)
	{
		materials.resize(0);
		materials.resize(mesh->subMeshCount, nullptr);
		materialSet.clear();
		materialSet.emplace(nullptr);
		MeshName = mesh->name;
		MatNames.clear();
		MatNames.resize(materials.size(), "DEFAULT MATERIAL");
		int i = 0;
		for(MaterialResource* elt : materials)
		{
			if(elt == nullptr)
			{
				++i;continue;
			}
			MatNames.at(i) = elt->name;
			++i;
		}

	}
}

Solid::MeshRenderer::MeshRenderer()
{


}

std::shared_ptr<IMesh> Solid::MeshRenderer::GetMesh() const
{
	return mesh;
}

void Solid::MeshRenderer::SetMesh(const std::shared_ptr<IMesh> _mesh)
{
	mesh = _mesh;
	MeshName =mesh->name;
	materialSet.clear();
	materials.resize(mesh->subMeshCount, nullptr);
	materialSet.insert(materials.begin(), materials.end());
	MatNames.clear();
	MatNames.resize(materials.size(), "DEFAULT MATERIAL");
	int i = 0;
	for(MaterialResource* elt : materials)
	{
	    if(elt == nullptr)
	    {
		    ++i;continue;
	    }
	    MatNames.at(i) = elt->name;
	    ++i;
	}

}

const std::vector<MaterialResource *> &Solid::MeshRenderer::GetMaterials()const
{
	return materials;
}

void MeshRenderer::SetMaterials(const std::vector<MaterialResource *> & _mats)
{
	materials = _mats;
	MatNames.clear();
	MatNames.resize(materials.size(), "DEFAULT MATERIAL");
	int i = 0;
	for(MaterialResource* elt : materials)
	{
		if(elt == nullptr)
		{
			++i;continue;
		}
		MatNames.at(i) = elt->name;
		++i;
	}
}

std::set<MaterialResource *> &Solid::MeshRenderer::GetMaterialSet()
{
	return materialSet;
}

void Solid::MeshRenderer::SetMaterialSet(const std::set<MaterialResource *> &_materialSet)
{
	materialSet = _materialSet;
}

void MeshRenderer::SetMaterialAt(int _index, MaterialResource *_mat)
{
	materials.at(_index) = _mat;
	materialSet.clear();
	materialSet.insert(materials.begin(), materials.end());
	MatNames.clear();
	MatNames.resize(materials.size(), "DEFAULT MATERIAL");
	int i = 0;
	for(MaterialResource* elt : materials)
	{
		if(elt == nullptr)
		{
			++i;continue;
		}
		MatNames.at(i) = elt->name;
		++i;
	}
}

void MeshRenderer::Init()
{
	mesh = Engine::GetInstance()->graphicsResourceMgr.GetMesh(MeshName.c_str());
	materials.clear();
	materials.resize(MatNames.size(), nullptr);
	materialSet.clear();
	for (int i = 0; i < MatNames.size(); ++i)
	{
		if(MatNames.at(i) == "DEFAULT MATERIAL")
		{
			continue;
		}
		materials.at(i) = Engine::GetInstance()->resourceManager.GetRawMaterialByName(MatNames.at(i).c_str());
	}
	materialSet.insert(materials.begin(), materials.end());
}

void MeshRenderer::Release()
{
	Components::Release();
	mesh = nullptr;
	materials.clear();
	materialSet.clear();
	MatNames.clear();
	MeshName.clear();
}
