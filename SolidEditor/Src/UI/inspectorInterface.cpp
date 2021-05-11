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

        if (openCreateScript)
            CreateScriptWindow();

	    DrawUniformNamePopup();
        UI::End();
    }

    void InspectorInterface::DrawComponents()
    {
        Engine*     engine = Engine::GetInstance();
        GameObject* gameObject = EditorInterface::selectedGO;

        UI::Text("Name: ");
        EditText(gameObject->name, "##name");

        //TODO: enable for custom scripts
        /*for(auto& comp : gameObject->compsList)
        {
            EditComp(comp);
        }*/

	    if(engine->ecsManager.GotComponent<Transform>(gameObject->GetEntity()))
	    {
		    EditTransform(engine->ecsManager.GetComponent<Transform>(gameObject->GetEntity()));
	    }

        if(engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
        {
            EditMeshRenderer(engine->ecsManager.GetComponent<MeshRenderer>(gameObject->GetEntity()));
        }

        if(engine->ecsManager.GotComponent<AudioSource>(gameObject->GetEntity()))
        {
            EditAudioSource(engine->ecsManager.GetComponent<AudioSource>(gameObject->GetEntity()));
        }

        if(engine->ecsManager.GotComponent<Animation>(gameObject->GetEntity()))
        {
            EditAnimation(engine->ecsManager.GetComponent<Animation>(gameObject->GetEntity()));
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
            if (UI::Button("Create Custom Script"))
                openCreateScript = true;
	        if(!engine->ecsManager.GotComponent<Transform>(gameObject->GetEntity()))
	        {
		        if(UI::Button("Transform"))
		        {
			        engine->ecsManager.AddComponent(gameObject,Transform());
                    UI::CloseCurrentPopup();
		        }
	        }
            if(!engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
            {
                if(UI::Button("Mesh renderer"))
                {
                    engine->ecsManager.AddComponent(gameObject,MeshRenderer());
                    UI::CloseCurrentPopup();
                }
            }
            if(!engine->ecsManager.GotComponent<AudioSource>(gameObject->GetEntity()))
            {
                if(UI::Button("Audio source"))
                {
                    engine->ecsManager.AddComponent<AudioSource>(gameObject,AudioSource());
                    UI::CloseCurrentPopup();
                }
            }
            if(!engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
            {
                if(UI::Button("RigidBody"))
                {
                    engine->ecsManager.AddComponent<RigidBody>(gameObject,RigidBody());
                    UI::CloseCurrentPopup();
                }
            }
            if(!engine->ecsManager.GotComponent<BoxCollider>(gameObject->GetEntity()))
            {
                if(UI::Button("Box collider"))
                {
                    engine->ecsManager.AddComponent<BoxCollider>(gameObject,BoxCollider());
                    UI::CloseCurrentPopup();
                }
            }
            if(!engine->ecsManager.GotComponent<SphereCollider>(gameObject->GetEntity()))
            {
                if(UI::Button("Sphere collider"))
                {
                    engine->ecsManager.AddComponent<SphereCollider>(gameObject,SphereCollider());
                    UI::CloseCurrentPopup();
                }
            }
            if(!engine->ecsManager.GotComponent<CapsuleCollider>(gameObject->GetEntity()))
            {
                if(UI::Button("Capsule collider"))
                {
                    engine->ecsManager.AddComponent<CapsuleCollider>(gameObject,CapsuleCollider());
                    UI::CloseCurrentPopup();
                }
            }

            if(!engine->ecsManager.GotComponent<Animation>(gameObject->GetEntity()))
            {
                if(UI::Button("Animation"))
                {
                    engine->ecsManager.AddComponent<Animation>(gameObject,Animation());
                    UI::CloseCurrentPopup();
                }
            }
            UI::EndPopup();
        }

    }

    void InspectorInterface::EditComp(Components* _comp)
    {
        if(UI::CollapsingHeader(_comp->getArchetype().name.c_str(),ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& f : _comp->getArchetype().fields)
            {
                std::string fieldName = f.name;
                std::string typeName = f.type.archetype->name;

                if(typeName == "bool")
                    EditBool(*(bool*)f.getDataAddress(_comp),fieldName);
                else if(typeName == "int")
                    EditInt(*(int*)f.getDataAddress(_comp),fieldName,1);
                else if(typeName == "float")
                    EditFloat(*(float*)f.getDataAddress(_comp),fieldName,0.01f);
                else if(typeName == "Vec2")
                    EditVec2(*((Vec2*)f.getDataAddress(_comp)),fieldName,0.01f);
                else if(typeName == "Vec3")
                    EditVec3(*((Vec3*)f.getDataAddress(_comp)),fieldName,0.01f);
                else if(typeName == "Vec4")
                    EditVec4(*((Vec4*)f.getDataAddress(_comp)),fieldName,0.01f);
                else if(typeName == "EditText")
                    EditText(*((std::string*)f.getDataAddress(_comp)),fieldName);
            }
        }
        UI::Separator();
    }

    void InspectorInterface::EditTransform(Transform& _trs)
    {
        if(UI::CollapsingHeader("Transform",ImGuiTreeNodeFlags_DefaultOpen))
        {
            UI::Indent();

            Vec3 tempPos   = _trs.GetPosition();
            Vec3 tempRot   = _trs.GetEuler();
            Vec3 tempScale = _trs.GetScale();

            UI::Text("Position");UI::SameLine();
	        EditVec3(tempPos, "##trsPos", 0.01f);
            UI::Text("Rotation");UI::SameLine();
            bool changed = EditVec3(tempRot, "##trsRot", 0.01f);
            UI::Text("Scale   ");UI::SameLine();
	        EditVec3(tempScale, "##trsScl", 0.01f);

            _trs.SetPosition(tempPos);
            if (changed)
            {
                _trs.SetEuler(tempRot);
            }
            _trs.SetScale(tempScale);
        }
        UI::Unindent();
        UI::Separator();
    }

    void InspectorInterface::EditMeshRenderer(MeshRenderer& _meshRenderer)
    {
        if(UI::CollapsingHeader("MeshRenderer",ImGuiTreeNodeFlags_DefaultOpen))
        {
            Engine* engine = Engine::GetInstance();
            auto mesh = _meshRenderer.GetMesh();
            const char* meshName = mesh == nullptr ? "" : mesh->name.c_str();

            UI::Text("Mesh  ");UI::SameLine();
            if(UI::BeginCombo("##Mesh", meshName))
            {
                auto* meshList = engine->resourceManager.GetResourcesVecByType<MeshResource>();

                for(auto mesh : *meshList)
                {
                    bool selected = (meshName == mesh.second->name);
                    if(UI::Selectable(mesh.second->name.c_str(), selected))
                    {
                        _meshRenderer.SetMesh( engine->graphicsResourceMgr.GetMesh(mesh.second->name.c_str()));

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
 			if(UI::TreeNode("Materials"))
 			{
			for(MaterialResource* elt : _meshRenderer.GetMaterials())
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

							_meshRenderer.SetMaterialAt(i, nullptr);
						}
						if(selected)
							UI::SetItemDefaultFocus();
					}
					for(auto mat : *matList)
					{
						bool selected = (matName == mat.second->name);
						if(UI::Selectable(mat.second->name.c_str(), selected))
						{
							_meshRenderer.SetMaterialAt(i, (MaterialResource*)mat.second);
						}
						if(selected)
							UI::SetItemDefaultFocus();
					}

					UI::EndCombo();
				}
				++i;
			}
			UI::TreePop();
			}
			i = 0;

			for(MaterialResource* elt : _meshRenderer.GetMaterialSet())
	        {
		        std::string matName = elt == nullptr ? "DEFAULT Name" :  elt->name;
	        	if(UI::TreeNode((matName + "##" + std::to_string(i)).c_str()))
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
							++ii;
				        }
				        ii = 0;
				        for(auto& fv : elt->ValuesProperties)
				        {
					        switch (fv.second.type)
					        {
					        	case MaterialResource::EFieldType::BOOL:
							        EditBool(fv.second.b, fv.first);
							        break;
						        case  MaterialResource::EFieldType::INT:
							        EditInt(fv.second.i, fv.first, 1);
							        break;
						        case  MaterialResource::EFieldType::FLOAT:
							        EditFloat(fv.second.f, fv.first, 0.01f);
							        break;
						        case  MaterialResource::EFieldType::VEC2:
							        EditVec2(fv.second.v2, fv.first,0.01f);
							        break;
						        case  MaterialResource::EFieldType::VEC3:
							        EditVec3(fv.second.v3, fv.first,0.01f);
							        break;
						        case  MaterialResource::EFieldType::VEC4:
							        EditVec4(fv.second.v4, fv.first,0.01f);
							        break;
						        default:
							        break;
					        }
							++ii;
				        }

				        if(UI::Button(("Add Texture##"+std::to_string(i)).c_str()))
				        {
					        textureAdd = true;
					        namePopup = true;
					        matToModify = elt;
				        }
				        if(UI::Button(("Add FieldValue##"+std::to_string(i)).c_str()))
				        {
					        fvAdd = true;
					        namePopup = true;
					        matToModify = elt;
				        }
			        }
                    UI::TreePop();
                }
		        ++i;
	        }
	        UI::Unindent();
        }
        UI::Separator();
    }

    void InspectorInterface::EditAudioSource(AudioSource &_audioSource)
    {
        Engine* engine = Engine::GetInstance();
        std::string audioName = _audioSource.GetName();

        if(UI::CollapsingHeader("Audio Source",ImGuiTreeNodeFlags_DefaultOpen))
        {
            if(UI::BeginCombo("##Audio", audioName.c_str()))
            {
                auto* audioList = engine->resourceManager.GetResourcesVecByType<AudioResource>();

                for(auto& audio : *audioList)
                {
                    bool selected = (audioName == audio.second->name);
                    if(UI::Selectable(audio.second->name.c_str(), selected))
                    {
                        _audioSource.SetAudio(engine->resourceManager.GetRawAudioByName(audio.second->name.c_str()));
                    }
                    if(selected)
                        UI::SetItemDefaultFocus();
                }

                UI::EndCombo();
            }
            
            {
                float volume = _audioSource.GetVolume();
                UI::SliderFloat("Volume",&volume,0,1);
                _audioSource.SetVolume(volume);
            }

            {
                float pitch = _audioSource.GetPitch();
                UI::SliderFloat("Pitch",&pitch,0,10,"%.2f");
                _audioSource.SetPitch(pitch);
            }

            {
                bool isLooping = _audioSource.IsLooping();
                UI::Checkbox("Loop",&isLooping);
                _audioSource.SetLoop(isLooping);
            }

            {
                float maxDist = _audioSource.GetMaxDistance();
                UI::DragFloat("MaxDist",&maxDist);
                _audioSource.SetMaxDistance(maxDist);
            }

            {
                Vec3 musicVelocity = _audioSource.GetMusicVelocity();
                UI::DragFloat3("Velocity",&musicVelocity.x);
                _audioSource.SetMusicVelocity(musicVelocity);
            }

            if(UI::Button("Play sound"))
                _audioSource.Play();
            if(UI::Button("Stop sound"))
                _audioSource.Stop();
        }

        UI::Separator();
    }

    void InspectorInterface::EditAnimation(Animation& _anim)
    {
        Engine* engine = Engine::GetInstance();

        if(UI::CollapsingHeader("Animation",ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char* SKName = _anim.GetAnim() == nullptr ? "NO ANIM" : _anim.GetAnim()->name.c_str();
            if(UI::BeginCombo("##ANIMATION", SKName))
            {
                auto* animList = engine->resourceManager.GetResourcesVecByType<AnimResource>();

                for(auto anim : *animList)
                {
                    bool selected = (SKName == anim.second->name);
                    if(UI::Selectable(anim.second->name.c_str(), selected))
                    {
                        _anim.SetAnim((AnimResource *) anim.second);

                    }
                    if(selected)
                        UI::SetItemDefaultFocus();
                }

                UI::EndCombo();
            }
        }

    }

    void InspectorInterface::CreateScriptWindow()
    {
        UI::SetNextWindowBgAlpha(0.9);
        UI::SetNextWindowSize(ImVec2(325, 100));
        UI::SetNextWindowViewport(UI::GetID(UI::GetMainViewport()));

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;

        static std::string sName = ""; //script name
        static std::string path  = "";  //path of the new script


        UI::Begin("Create Script", &openCreateScript, flags);

        UI::Text("Script Name:");
        UI::SameLine();
        ImVec2 cPos = UI::GetCursorPos();
        UI::InputText("##InputScriptName:", &sName);


        UI::Text("Path:");
        UI::SameLine();
        cPos.y = UI::GetCursorPos().y;
        UI::SetCursorPos(cPos);
        UI::InputText("##InputPath:", &path);

        UI::Indent(50.f);
        if(UI::Button("Create"))
        {
            openCreateScript = false;
            //call function to create custom script here
            path = "";
            sName = "";
        }
        UI::SameLine();
        UI::Indent(125.f);
        if (UI::Button("Cancel"))
        {
            openCreateScript = false;
            path = "";
            sName = "";
        }
        UI::End();
    }

	void InspectorInterface::DrawUniformNamePopup()
	{
		if(namePopup)
		{
			UI::OpenPopup("ValueNamePopUp");
			namePopup = false;
			nameStr = "";
			beginField = "NONE";
			chosenType =(int)MaterialResource::EFieldType::NONE;
		}
		if(!UI::IsPopupOpen("ValueNamePopUp"))
		{
			textureAdd = false;
			fvAdd = false;
		}
		if (UI::BeginPopup("ValueNamePopUp"))
		{
			UI::Text("Value Name :");
			UI::SameLine();
			UI::InputText("##ValueName", &nameStr);
			if(textureAdd)
			{

				if (UI::Button("Add Texture"))
				{
					matToModify->TexturesProperties.emplace(nameStr, nullptr);
					textureAdd = false;
					matToModify = nullptr;
					UI::CloseCurrentPopup();
				}


			}
			else
			{
				const char* fieldType[(int)MaterialResource::EFieldType::NONE] = {"Bool", "Int", "Float", "Vec2", "Vec3", "Vec4", "NONE"};

				if(UI::BeginCombo("Wanted Field ##ChooseField",beginField))
				{

					for(int i = 0; i < (int)MaterialResource::EFieldType::NONE; ++i)
					{
						bool selected = (fieldType[i] == beginField);
						if(UI::Selectable(fieldType[i], selected))
						{
							beginField = fieldType[i];
							chosenType = i+1;
						}
						if(selected)
							UI::SetItemDefaultFocus();
					}
					UI::EndCombo();
				}

				if(UI::Button("Add Field"))
				{


						matToModify->ValuesProperties.emplace(nameStr, (MaterialResource::EFieldType)chosenType);
						fvAdd = false;
						matToModify = nullptr;

					UI::CloseCurrentPopup();

				}

			}
			UI::EndPopup();
		}



	}

	void InspectorInterface::EditBool(bool &_num, const std::string &_label)
	{
		if(UI::Checkbox(_label.c_str(), &_num))
		{

		}
	}

    void InspectorInterface::EditInt(int& _num, const std::string& _label, float _step)
    {
        UI::DragInt(_label.c_str(), &_num, _step);
    }

    void InspectorInterface::EditFloat(float &_num, const std::string& _label, float _step)
    {
        UI::DragFloat(_label.c_str(), &_num, _step);
    }

    void InspectorInterface::EditVec2(Vec2& _vec, const std::string& _label, float _step)
    {
        UI::DragFloat2(_label.c_str(), &_vec.x, _step);
    }

	bool InspectorInterface::EditVec3(Vec3 &_vec, const std::string &_label, float _step)
    {
        Vec3 temp = _vec;
        if(UI::DragFloat3(_label.c_str(), &temp.x, _step))
        {
            _vec = temp;
            return true;
        }
        return false;
    }
	bool InspectorInterface::EditVec4(Vec4 &_vec, const std::string &_label, float _step)
	{
		Vec4 temp = _vec;
		if(UI::DragFloat4(_label.c_str(), &temp.x, _step))
		{
			_vec = temp;
			return true;
		}
		return false;
	}

    void InspectorInterface::EditText(std::string &_str, const std::string& _label)
    {
        UI::InputText(_label.c_str(), &_str);
    }
}
