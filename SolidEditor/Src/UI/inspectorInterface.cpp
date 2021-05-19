#include "UI/inspectorInterface.hpp"
#include "UI/editorInterface.hpp"
#include "Core/engine.hpp"
#include "editor.hpp"
#include "ECS/Components/script.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include "ECS/Components/scriptList.hpp"

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
        
        if(engine->ecsManager.GotComponent<ScriptList>(gameObject->GetEntity()))
        {
	        ScriptList& sl = engine->ecsManager.GetComponent<ScriptList>(gameObject->GetEntity());
	        for(Script* elt : sl.GetAllScripts())
	        {
		        EditComp(elt);
	        }
	    }

        if(engine->ecsManager.GotComponent<Light>(gameObject->GetEntity()))
        {
            EditLight(engine->ecsManager.GetComponent<Light>(gameObject->GetEntity()));
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
	    if(UI::Button("Remove Component",ImVec2(-1, 0)))
		    UI::OpenPopup("RemoveComponent");
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
            if(!engine->ecsManager.GotComponent<Light>(gameObject->GetEntity()))
            {
                if(UI::Button("Light"))
                {
                    engine->ecsManager.AddComponent<Light>(gameObject,Light());
                    UI::CloseCurrentPopup();
                }
            }
	        if(!engine->ecsManager.GotComponent<ScriptList>(gameObject->GetEntity()))
	        {
	        	const rfk::Namespace* n = GameCompiler::GetInstance()->GetNamespace("Solid");
	        	if(n!= nullptr)
		        {
			        for(auto& elt : n->archetypes)
			        {
				        const rfk::Class* c = n->getClass(elt->name);
				        if(c->isSubclassOf(*n->getClass("Script")))
				        {
					        if(UI::Button(elt->name.c_str()))
					        {
						        ScriptList* sl =engine->ecsManager.AddComponent<ScriptList>(gameObject, ScriptList());
						        sl->AddScript(c->makeInstance<Script>());
						        UI::CloseCurrentPopup();
					        }
				        }

			        }
		        }
	        }
	        else
	        {
		        const rfk::Namespace* n = GameCompiler::GetInstance()->GetNamespace("Solid");
		        if(n != nullptr)
		        {
			        ScriptList& sl =engine->ecsManager.GetComponent<ScriptList>(gameObject->GetEntity());

			        for(auto& elt : n->archetypes)
			        {
				        if(!sl.HasScript(elt->name.c_str()))
				        {
					        const rfk::Class* c = n->getClass(elt->name);
					        if(c->isSubclassOf(*n->getClass("Script")))
					        {
						        if(UI::Button(elt->name.c_str()))
						        {
							        sl.AddScript(c->makeInstance<Script>());
							        UI::CloseCurrentPopup();
						        }
					        }
				        }


			        }
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
	    if(UI::BeginPopup("RemoveComponent"))
	    {
		    if(engine->ecsManager.GotComponent<Transform>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Transform"))
			    {
				    engine->ecsManager.RemoveComponent<Transform>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Mesh renderer"))
			    {
				    engine->ecsManager.RemoveComponent<MeshRenderer>(gameObject);

				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<AudioSource>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Audio source"))
			    {
				    engine->ecsManager.RemoveComponent<AudioSource>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<RigidBody>(gameObject->GetEntity()))
		    {
			    if(UI::Button("RigidBody"))
			    {
				    engine->ecsManager.RemoveComponent<RigidBody>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<BoxCollider>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Box collider"))
			    {
				    engine->ecsManager.RemoveComponent<BoxCollider>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<SphereCollider>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Sphere collider"))
			    {
				    engine->ecsManager.RemoveComponent<SphereCollider>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<CapsuleCollider>(gameObject->GetEntity()))
		    {
			    if(UI::Button("Capsule collider"))
			    {
				    engine->ecsManager.RemoveComponent<CapsuleCollider>(gameObject);
				    UI::CloseCurrentPopup();
			    }
		    }
		    if(engine->ecsManager.GotComponent<ScriptList>(gameObject->GetEntity()))
		    {
		    	ScriptList& sl = engine->ecsManager.GetComponent<ScriptList>(gameObject->GetEntity());
		    	std::vector<Script*>& sv = sl.GetAllScripts();
			    for(Script* elt : sv)
			    {
				    if(UI::Button(elt->getArchetype().name.c_str()))
				    {
					    sl.RemoveScript(elt);
					    UI::CloseCurrentPopup();
				    }
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
                else if(typeName == "String")
                    EditText(*((String*)f.getDataAddress(_comp)),fieldName);
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

			bool combo =UI::BeginCombo("##Mesh", meshName);
	        if(UI::BeginDragDropTarget())
	        {

		        const ImGuiPayload* drop=UI::AcceptDragDropPayload("Mesh");
		        if(drop != nullptr)
		        {
			        std::string s = std::string((char*)drop->Data, drop->DataSize);
			        _meshRenderer.SetMesh(engine->graphicsResourceMgr.GetMesh(s.c_str()));

		        }
		        UI::EndDragDropTarget();
	        }
            if(combo)
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

            if(UI::TreeNode("Used Materials"))
            {
                UI::Indent();

                int id = 0;
                for (auto* mat : _meshRenderer.GetMaterials())
                {
                    std::string matName = mat == nullptr ? "Default Material" : mat->name;
                    std::string matId = std::to_string(id);

                    // Choose Material
                    UI::Text(("Mat "+matId).c_str());UI::SameLine();
					bool combo =UI::BeginCombo(std::string("##Mat"+matId).c_str(),matName.c_str());
	                if(UI::BeginDragDropTarget())
	                {

		                const ImGuiPayload* drop=UI::AcceptDragDropPayload("Material");
		                if(drop != nullptr)
		                {
			                std::string s = std::string((char*)drop->Data, drop->DataSize);
			                _meshRenderer.SetMaterialAt(id, engine->resourceManager.GetRawMaterialByName(s.c_str()));

		                }
		                UI::EndDragDropTarget();
	                }
                    if(combo)
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
                    ++id;
                }

                UI::Unindent();
                UI::TreePop();
            }

            // Edit Material
            if(UI::TreeNode("Edit Materials"))
            {
                UI::Indent();

                int Id = 0;
                auto matSet = _meshRenderer.GetMaterialSet();
                for(auto* mat : matSet)
                {
                    if(mat == nullptr)
                        continue;

                    std::string matId = std::to_string(Id);

                    if(UI::TreeNode(mat->name.c_str()))
                    {
                        if(UI::Button("Edit vertex shader"))
                        {
                            codeEditor.isCodeEditorOpen = true;
                            codeEditor.imCodeEditor.SetText(mat->GetShader()->GetVertSource());
                            codeEditor.codeType = CodeEditor::ECodeType::VERTEX;
                        }
                        if(UI::Button("Edit fragment shader"))
                        {
                            codeEditor.isCodeEditorOpen = true;
                            codeEditor.imCodeEditor.SetText(mat->GetShader()->GetFragSource());
                            codeEditor.codeType = CodeEditor::ECodeType::FRAGMENT;
                        }

                        //Open window to edit shader
                        if(codeEditor.isCodeEditorOpen)
                        {
                            auto cpos = codeEditor.imCodeEditor.GetCursorPosition();
                            UI::Begin("Edit shader##Window", &codeEditor.isCodeEditorOpen,ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
                            if (ImGui::BeginMenuBar())
                            {
                                if (ImGui::BeginMenu("File"))
                                {
                                    if (ImGui::MenuItem("Save","Ctrl-S"))
                                    {
                                        if(codeEditor.codeType == CodeEditor::ECodeType::FRAGMENT)
                                            mat->GetShader()->SetFragSource(codeEditor.imCodeEditor.GetText());
                                        else if(codeEditor.codeType == CodeEditor::ECodeType::VERTEX)
                                            mat->GetShader()->SetVertSource(codeEditor.imCodeEditor.GetText());

                                        mat->GetShader()->ReloadShader();
                                        mat->LoadShaderFields();
                                    }
                                    if (ImGui::MenuItem("Quit", "Alt-F4"))
                                        codeEditor.isCodeEditorOpen = false;
                                    ImGui::EndMenu();
                                }
                                if (ImGui::BeginMenu("Edit"))
                                {
                                    if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, codeEditor.imCodeEditor.CanUndo()))
                                        codeEditor.imCodeEditor.Undo();
                                    if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, codeEditor.imCodeEditor.CanRedo()))
                                        codeEditor.imCodeEditor.Redo();

                                    ImGui::Separator();

                                    if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, codeEditor.imCodeEditor.HasSelection()))
                                        codeEditor.imCodeEditor.Copy();
                                    if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, codeEditor.imCodeEditor.HasSelection()))
                                        codeEditor.imCodeEditor.Cut();
                                    if (ImGui::MenuItem("Delete", "Del", nullptr, codeEditor.imCodeEditor.HasSelection()))
                                        codeEditor.imCodeEditor.Delete();
                                    if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, ImGui::GetClipboardText() != nullptr))
                                        codeEditor.imCodeEditor.Paste();

                                    ImGui::Separator();

                                    if (ImGui::MenuItem("Select all", nullptr, nullptr))
                                        codeEditor.imCodeEditor.SelectAll();

                                    ImGui::EndMenu();
                                }

                                if (ImGui::BeginMenu("View"))
                                {
                                    if (ImGui::MenuItem("Dark palette"))
                                        codeEditor.imCodeEditor.SetPalette(TextEditor::GetDarkPalette());
                                    if (ImGui::MenuItem("Light palette"))
                                        codeEditor.imCodeEditor.SetPalette(TextEditor::GetLightPalette());
                                    ImGui::EndMenu();
                                }
                                ImGui::EndMenuBar();
                            }

                            ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.imCodeEditor.GetTotalLines(),
                                        codeEditor.imCodeEditor.IsOverwrite() ? "Ovr" : "Ins",
                                        codeEditor.imCodeEditor.CanUndo() ? "*" : " ",
                                        codeEditor.imCodeEditor.GetLanguageDefinition().mName.c_str(), mat->GetShader()->name.c_str());

                            codeEditor.imCodeEditor.Render("Edit shader",UI::GetContentRegionAvail(), false);
                            UI::End();
                        }

                        // Choose Shader
                        const char* shaderName = mat->GetShader()->name.c_str();
                        bool combo =UI::BeginCombo(std::string("Shader##"+matId).c_str(),shaderName);
	                    if(UI::BeginDragDropTarget())
	                    {

		                    const ImGuiPayload* drop=UI::AcceptDragDropPayload("Shader");
		                    if(drop != nullptr)
		                    {
			                    std::string s = std::string((char*)drop->Data, drop->DataSize);
			                    mat->SetShader(engine->graphicsResourceMgr.GetShader(s.c_str()));

		                    }
		                    UI::EndDragDropTarget();
	                    }
                        if(combo)
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

                        // Edit Shader
                        if(UI::TreeNode(std::string("Edit "+std::string(shaderName) + "##" + matId).c_str()))
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
                    ++Id;
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
        	bool combo = UI::BeginCombo("##Audio", audioName.c_str());
	        if(UI::BeginDragDropTarget())
	        {

		        const ImGuiPayload* drop=UI::AcceptDragDropPayload("Audio");
		        if(drop != nullptr)
		        {
			        std::string s = std::string((char*)drop->Data, drop->DataSize);
			        _audioSource.SetAudio(engine->resourceManager.GetRawAudioByName(s.c_str()));

		        }
		        UI::EndDragDropTarget();
	        }
            if(combo)
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

    void InspectorInterface::EditLight(Light &_light)
    {
        Engine* engine = Engine::GetInstance();

        if(UI::CollapsingHeader("Light",ImGuiTreeNodeFlags_DefaultOpen))
        {
            UI::ColorEdit3("Color",&_light.color.x);
            EditFloat(_light.intensity,"Intensity",0.01f);
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
	    if(UI::BeginDragDropTarget())
	    {

		    const ImGuiPayload *drop = UI::GetDragDropPayload();
		    if (drop != nullptr)
		    {
			    _str = std::string((char *) drop->Data, drop->DataSize);


		    }
		    UI::EndDragDropTarget();
	    }
    }

    void InspectorInterface::EditTexture(std::shared_ptr<ITexture>& _texture, const std::string &_label)
    {
        Engine *engine = Engine::GetInstance();

        std::string textName = _texture == nullptr ? "None" : _texture->name;

	    bool combo = UI::BeginCombo(_label.c_str(),textName.c_str());
	    if(UI::BeginDragDropTarget())
	    {

		    const ImGuiPayload* drop=UI::AcceptDragDropPayload("Image");
		    if(drop != nullptr)
		    {
			    std::string s = std::string((char*)drop->Data, drop->DataSize);
			    _texture = engine->graphicsResourceMgr.GetTexture(s.c_str());

		    }
		    UI::EndDragDropTarget();
	    }
        if (combo)
        {
            auto *textures = engine->resourceManager.GetResourcesVecByType<ImageResource>();
            for (auto text : *textures)
            {
                bool selected = (textName == text.second->name);
                if(UI::Selectable(text.second->name.c_str(), selected))
                {
                    _texture = engine->graphicsResourceMgr.GetTexture(text.second->name.c_str());
                }
                if(selected)
                    UI::SetItemDefaultFocus();
            }

            UI::EndCombo();
        }
        //UI::Image(engine->graphicsResourceMgr.GetTexture(_texture->name))
    }

} //namespace
