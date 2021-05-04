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

            if(UI::TreeNode("Materials"))
            {
                UI::Indent();
                int id = 0;
                for (auto* mat : _meshRenderer.GetMaterials())
                {
                    std::string matName = mat == nullptr ? "Default Material" : mat->name;
                    std::string matId = std::to_string(id);

                    if(UI::TreeNode(matName.c_str()))
                    {
                        // Choose Material
                        if(UI::BeginCombo(std::string("##Mat"+matId).c_str(),matName.c_str()))
                        {
                            auto* matList = engine->resourceManager.GetResourcesVecByType<MaterialResource>();
                            {
                                bool selected = (matName == "DEFAULT MATERIAL");
                                if(UI::Selectable("DEFAULT MATERIAL", selected))
                                {

                                    _meshRenderer.SetMaterialAt(id, nullptr);
                                }
                                if(selected)
                                    UI::SetItemDefaultFocus();
                            }
                            for(auto mat : *matList)
                            {
                                bool selected = (matName == mat.second->name);
                                if(UI::Selectable(mat.second->name.c_str(), selected))
                                {
                                    _meshRenderer.SetMaterialAt(id, (MaterialResource*)mat.second);
                                }
                                if(selected)
                                    UI::SetItemDefaultFocus();
                            }

                            UI::EndCombo();
                        }

                        // Choose Shader
                        const char* shaderName = mat == nullptr ? "Default Shader" : mat->GetShader()->name.c_str();
                        if(UI::BeginCombo(std::string("##Shader"+matId).c_str(),shaderName))
                        {
                            auto* shaderList = engine->resourceManager.GetResourcesVecByType<ShaderResource>();
                            for(auto shader : *shaderList)
                            {
                                bool selected = (shaderName == shader.second->name);
                                if(UI::Selectable(shader.second->name.c_str(), selected))
                                {
                                    mat->SetShader(engine->graphicsResourceMgr.GetShader(shader.second->name.c_str()));
                                }
                                if(selected)
                                    UI::SetItemDefaultFocus();
                            }
                            UI::EndCombo();
                        }

                        // Edit shader
                        if(UI::TreeNode(std::string("Edit "+std::string(shaderName)+"##"+matId).c_str()))
                        {
                            for(auto& field : mat->fields)
                            {
                                switch(field.type)
                                {
                                    case MaterialResource::EShaderFieldType::BOOL:
                                        EditBool(field.b,field.name);
                                        break;
                                    case MaterialResource::EShaderFieldType::INT:
                                        EditInt(field.i,field.name,1);
                                        break;
                                    case MaterialResource::EShaderFieldType::FLOAT:
                                        EditFloat(field.f,field.name,0.01f);
                                        break;
                                    case MaterialResource::EShaderFieldType::VEC2:
                                        EditVec2(field.v2,field.name,0.01f);
                                        break;
                                    case MaterialResource::EShaderFieldType::VEC3:
                                        EditVec3(field.v3,field.name,0.01f);
                                        break;
                                    case MaterialResource::EShaderFieldType::VEC4:
                                        EditVec4(field.v4,field.name,0.01f);
                                        break;
                                    case MaterialResource::EShaderFieldType::TEXT:
                                        EditTexture(field.text,field.name);
                                        break;
                                    default:
                                        break;
                                }
                            }
                            UI::TreePop();
                        }

                        UI::TreePop();
                    }

                    ++id;
                }

                UI::Unindent();
                UI::TreePop();
            }
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

    void InspectorInterface::EditTexture(const std::shared_ptr<ITexture> _texture, const std::string &_label)
    {
        Engine* engine = Engine::GetInstance();
        //UI::Image(engine->graphicsResourceMgr.GetTexture(_texture->name))
    }
}
