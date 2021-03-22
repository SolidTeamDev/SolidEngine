#include "UI/inspectorInterface.hpp"
#include "UI/editorInterface.hpp"
#include "Core/engine.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
namespace Solid
{
    void InspectorInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Inspector", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        if(EditorInterface::selectedGO == nullptr)
        {
            UI::End();
            return;
        }

        DrawComponents();

        AddComponents();
	    DrawUniformNamePopup();
        UI::End();
    }

    void InspectorInterface::DrawComponents()
    {
        Engine*     engine = Engine::GetInstance();
        GameObject* gameObject = EditorInterface::selectedGO;

        UI::Text("Name: ");
        EditText(gameObject->name, "##name");
	    if(engine->ecsManager.GotComponent<Transform>(gameObject->GetEntity()))
	    {
		    EditTransform(engine->ecsManager.GetComponent<Transform>(gameObject->GetEntity()));
	    }


        if(engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
        {
            EditMeshRenderer(engine->ecsManager.GetComponent<MeshRenderer>(gameObject->GetEntity()));
        }
    }

    void InspectorInterface::AddComponents()
    {
        Engine*     engine = Engine::GetInstance();
        GameObject* gameObject = EditorInterface::selectedGO;

        if(UI::Button("Add Component",ImVec2(-1, 0)))
            UI::OpenPopup("AddComponent");

        if(UI::BeginPopup("AddComponent"))
        {
	        if(!engine->ecsManager.GotComponent<Transform>(gameObject->GetEntity()))
	        {
		        if(UI::Button("Transform"))
		        {
			        engine->ecsManager.AddComponent(gameObject->GetEntity(),Transform());
		        }
	        }
            if(!engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
            {
                if(UI::Button("Mesh renderer"))
                {
                    engine->ecsManager.AddComponent(gameObject->GetEntity(),MeshRenderer());
                }
            }
            UI::EndPopup();
        }

    }

    void InspectorInterface::EditTransform(Transform& _trs)
    {
        ImVec2 pos = UI::GetCursorPos();
        pos.y += 20;
        UI::SetCursorPos(pos);

        if(UI::CollapsingHeader("Transform",ImGuiTreeNodeFlags_DefaultOpen))
        {
            Vec3 tempPos   = _trs.GetPosition();
            Vec3 tempRot   = _trs.GetEuler();
            Vec3 tempScale = _trs.GetScale();

            UI::Text("Position");UI::SameLine();
            EditVec3(tempPos  , "##trsPos", 0.02f);
            UI::Text("Rotation");UI::SameLine();
            bool changed = EditVec3(tempRot  , "##trsRot", 1.f);
            UI::Text("Scale   ");UI::SameLine();
            EditVec3(tempScale, "##trsScl", 0.01f);

            _trs.SetPosition(tempPos);
            if (changed)
            {
                _trs.SetEuler(tempRot);
            }
            _trs.SetScale(tempScale);
        }
    }

    void InspectorInterface::EditMeshRenderer(MeshRenderer& _meshRenderer)
    {
        if(UI::CollapsingHeader("MeshRenderer",ImGuiTreeNodeFlags_DefaultOpen))
        {
            Engine* engine = Engine::GetInstance();
            const char* meshName = _meshRenderer.mesh == nullptr ? "" : _meshRenderer.mesh->name.c_str();

            UI::Text("Mesh  ");UI::SameLine();
            if(UI::BeginCombo("##Mesh", meshName))
            {
                auto* meshList = engine->resourceManager.GetResourcesVecByType<MeshResource>();

                for(auto mesh : *meshList)
                {
                    bool selected = (meshName == mesh.second->name);
                    if(UI::Selectable(mesh.second->name.c_str(), selected))
                    {
                        _meshRenderer.mesh = engine->graphicsResourceMgr.GetMesh(mesh.second->name.c_str());
	                    //_meshRenderer.materials.clear();
						_meshRenderer.materialSet.clear();
	                    _meshRenderer.materials.resize(_meshRenderer.mesh->subMeshCount, nullptr);
                        _meshRenderer.materialSet.insert(_meshRenderer.materials.begin(), _meshRenderer.materials.end());
                    }
                    if(selected)
                        UI::SetItemDefaultFocus();
                }

                UI::EndCombo();
            }
            int i = 0;
            const MaterialResource* DefaultMat = engine->resourceManager.GetDefaultMat();
            auto Lambda = [&](auto elt){

            };
			UI::Indent();
 			if(UI::CollapsingHeader("Materials"))
 			{
			for(MaterialResource* elt : _meshRenderer.materials)
			{
				const char* matName = elt == nullptr ? "DEFAULT MATERIAL" :  elt->name.c_str();
				UI::Text("Material  ");UI::SameLine();
				if(UI::BeginCombo(("##Mat" + std::to_string(i)).c_str(), matName))
				{
					auto* matList = engine->resourceManager.GetResourcesVecByType<MaterialResource>();
					{
						bool selected = (matName == "DEFAULT MATERIAL");
						if(UI::Selectable("DEFAULT MATERIAL", selected))
						{

							_meshRenderer.materials.at(i) = nullptr;
							_meshRenderer.materialSet.clear();
							_meshRenderer.materialSet.insert(_meshRenderer.materials.begin(), _meshRenderer.materials.end());
						}
						if(selected)
							UI::SetItemDefaultFocus();
					}
					for(auto mat : *matList)
					{
						bool selected = (matName == mat.second->name);
						if(UI::Selectable(mat.second->name.c_str(), selected))
						{

							_meshRenderer.materials.at(i) = (MaterialResource*)mat.second;
							_meshRenderer.materialSet.clear();
							_meshRenderer.materialSet.insert(_meshRenderer.materials.begin(), _meshRenderer.materials.end());
						}
						if(selected)
							UI::SetItemDefaultFocus();
					}

					UI::EndCombo();
				}
				++i;
			}
			}
			i = 0;

			for(MaterialResource* elt : _meshRenderer.materialSet)
	        {
		        std::string matName = elt == nullptr ? "DEFAULT Name" :  elt->name;
	        	if(UI::CollapsingHeader((matName + "##" + std::to_string(i)).c_str()))
		        {

	        		if(elt == nullptr)
			        {
	        			/// const view / no modifying possible
			        }
	        		else
			        {
				        {
				        	///shader modifying

				        	const char* shaderName = elt->shader == nullptr ? "" : elt->shader->name.c_str();
					        if(UI::BeginCombo(("Used Shader##ChooseShader" +  std::to_string(i)).c_str(),shaderName))
					        {
						        auto* shaderList = engine->resourceManager.GetResourcesVecByType<ShaderResource>();
						        for(auto shader : *shaderList)
						        {
							        bool selected = (meshName == shader.second->name);
							        if(UI::Selectable(shader.second->name.c_str(), selected))
							        {
								        elt->shader = engine->graphicsResourceMgr.GetShader(shader.second->name.c_str());
							        }
							        if(selected)
								        UI::SetItemDefaultFocus();
						        }
						        UI::EndCombo();
					        }


				        }
	        			for(auto& value : elt->ValueProperties)
	        			{
							EditFloat(value.second, value.first);
	        			}
				        for(auto& color : elt->ColorProperties)
				        {
					        EditVec4(color.second, color.first);
				        }
				        int ii = 0;
				        for(auto& tex : elt->TexturesProperties)
				        {
				        	const char* texName = tex.second == nullptr ? "" : tex.second->name.c_str();
					        if(UI::BeginCombo((tex.first +"##ChooseTex"+ std::to_string(i)+"_" + std::to_string(ii)).c_str(),texName))
					        {
						        auto* ImageList = engine->resourceManager.GetResourcesVecByType<ImageResource>();
						        for(auto& image : *ImageList)
						        {
							        bool selected = (meshName == image.second->name);
							        if(UI::Selectable(image.second->name.c_str(), selected))
							        {
								        tex.second = engine->graphicsResourceMgr.GetTexture(image.second->name.c_str());
							        }
							        if(selected)
								        UI::SetItemDefaultFocus();
						        }
						        UI::EndCombo();
					        }
							++i;
				        }
	        			if(UI::Button(("Add Color##"+std::to_string(i)).c_str()))
				        {
	        				colorAdd = true;
	        				namePopup = true;
	        				matToModify = elt;
				        }
				        if(UI::Button(("Add Texture##"+std::to_string(i)).c_str()))
				        {
					        textureAdd = true;
					        namePopup = true;
					        matToModify = elt;
				        }
				        if(UI::Button(("Add Float##"+std::to_string(i)).c_str()))
				        {
					        floatAdd = true;
					        namePopup = true;
					        matToModify = elt;
				        }
			        }
		        }
		        ++i;
	        }
	        UI::Unindent();
        }
    }

	void InspectorInterface::DrawUniformNamePopup()
	{
		if(namePopup)
		{
			UI::OpenPopup("ValueNamePopUp");
			namePopup = false;
			nameStr = "";
		}
		if(!UI::IsPopupOpen("ValueNamePopUp"))
		{
			colorAdd = false;
			textureAdd = false;
			floatAdd = false;
		}
		if (UI::BeginPopup("ValueNamePopUp"))
		{
			UI::Text("Value Name :");
			UI::SameLine();
			UI::InputText("##ValueName", &nameStr);
			if(UI::Button("Add Value"))
			{
				if(colorAdd)
				{
					matToModify->ColorProperties.emplace(nameStr, Vec4());
					colorAdd = false;
					matToModify = nullptr;
				}
				else if (textureAdd)
				{
					matToModify->TexturesProperties.emplace(nameStr, nullptr);
					textureAdd = false;
					matToModify = nullptr;
				}
				else if(floatAdd)
				{
					matToModify->ValueProperties.emplace(nameStr, 0.0f);
					floatAdd= false;
					matToModify = nullptr;
				}
				UI::CloseCurrentPopup();
			}
			UI::EndPopup();
		}

	}

    bool InspectorInterface::EditVec3(Vec3& _vec, const std::string& _label)
    {
        Vec3 temp = _vec;
        if(UI::DragFloat3(_label.c_str(), &temp.x, _step))
        {
            _vec = temp;
            return true;
        }
        return false;
    }
	bool InspectorInterface::EditVec4(Vec4& _vec, const std::string& _label)
	{
		Vec4 temp = _vec;
		if(UI::DragFloat4(_label.c_str(), &temp.x, 0.01f))
		{
			_vec = temp;
			return true;
		}
		return false;
	}

    void InspectorInterface::EditVec2(Vec2& _vec, const std::string& _label, float _step)
    {
        UI::DragFloat2(_label.c_str(), &_vec.x, _step);
    }

    void InspectorInterface::EditInt(int& _num, const std::string& _label, float _step)
    {
        UI::DragInt(_label.c_str(), &_num, _step);
    }

    void InspectorInterface::EditFloat(float &_num, const std::string& _label, float _step)
    {
        UI::DragFloat(_label.c_str(), &_num, _step);
    }

    void InspectorInterface::EditText(std::string &_str, const std::string& _label)
    {
        UI::InputText(_label.c_str(), &_str);
    }
}
