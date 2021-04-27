#include "UI/editorInterface.hpp"
#include "UI/solidUI.hpp"
#include "Time/time.hpp"
#include "editor.hpp"

#include <imgui_internal.h>
#include <string>
#include <sstream>


Solid::GameObject* Solid::EditorInterface::selectedGO = nullptr;
bool               Solid::EditorInterface::draggingEnt = false;

namespace Solid {
    EditorInterface::EditorInterface() :
            editorStyle(UI::GetStyle())
    {
        window = nullptr;
        DarkTheme();
    }

    EditorInterface::EditorInterface(Window *_window) :
            editorStyle(UI::GetStyle())
    {
        window = _window;
        DarkTheme();
    }

    void EditorInterface::Update()
    {
        UIContext::BeginFrame();

        DrawMainFrame();

        filesInterface.Draw();
        sceneInterface.Draw();
        inspectorInterface.Draw();
        hierarchyTreeInterface.Draw();
        playInterface.Draw();
        logsInterface.Draw();

        if (colorOpen)
            DrawChangeColors();
        if (perfOpen)
            AddInfoOverlay();
        if (demoOpen)
            UI::ShowDemoWindow();


        UIContext::RenderFrame();
    }

#pragma region MainMenuBar
    void EditorInterface::DrawMenuBar()
    {
        if (window == nullptr)
            return;

        if (UI::BeginMainMenuBar())
        {
            DrawMenuFiles();
            DrawMenuWindows();

            UI::EndMainMenuBar();
        }

    }

    void EditorInterface::DrawMenuFiles()
    {
        if (UI::BeginMenu("Files"))
        {

            if (UI::MenuItem("Save Scene"))
            {
	            json j;
	            j["Scene"].array();
	            j = j.flatten();
	            std::string elt = "/Scene";
	            GameObject* world = Engine::GetInstance()->ecsManager.GetWorld();
	            std::function<void(json&, GameObject*, std::string&)> Lambda = [&](json& j, GameObject* elt, std::string& path){

		            for(GameObject* sub : elt->childs)
		            {
			            std::string subP = path + "/{GameObject_"+ std::to_string(sub->GetEntity()) + "}" ;
			            j[subP + "/Name"] = sub->name;
			            Lambda(std::ref(j), sub, std::ref(subP));
		            }

	            };
	            Lambda(std::ref(j), world, std::ref(elt));
	            j = j.unflatten();
				std::string name = "test.SolidScene";
	            std::ofstream file(name, std::ifstream::binary);
	            std::vector<char> buffer;
	            std::stringstream sstr;
	            sstr << std::setw(4) << j << std::endl;
	            std::size_t sstrSize = sstr.str().size() * sizeof(std::string::value_type);
	            ResourcesLoader::Append(buffer, &sstrSize , sizeof(std::size_t));

	            ResourcesLoader::Append(buffer, sstr.str().data(), sstrSize);
	            std::function<void(GameObject*)> LambdaCmp = [&](GameObject* elt){
		            //store Num of Childs
	            	std::size_t ChildNum = elt->childs.size();
		            ResourcesLoader::Append(buffer, &ChildNum, sizeof(std::size_t));
		            for(GameObject* sub : elt->childs)
		            {
						//store num of comps
		            	std::size_t cmpNum = sub->compsList.size();
			            ResourcesLoader::Append(buffer, &cmpNum, sizeof(std::size_t));
						for(Components* cmp : sub->compsList)
						{
						    Log::Send(cmp->getArchetype().name);
							std::size_t offset =0;

							std::size_t cmpNameSize = 0;

							//store comp name / string
							cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
							ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);

							//store num of fields
							std::size_t numFields = cmp->getArchetype().fields.size();
							ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
							for(auto& elt : cmp->getArchetype().fields)//2 elt var WARN
						    {
								std::size_t size = 0;
								size = elt.name.size()*sizeof(std::string::value_type);
							    //store field name / string
								ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							    ResourcesLoader::Append(buffer, (void*)elt.name.data(),  size);
							    short isNull = 128;
								if(elt.type.archetype == nullptr)
								{
									isNull = 256;
									std::string str = elt.getData<std::string>(cmp);
									std::size_t strS =  str.size()*sizeof(std::string::value_type);
									//store isNull
									ResourcesLoader::Append(buffer, &isNull, sizeof(short));
									//store field data
									ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, str.data(), strS);
								}
								else
								{
									ResourcesLoader::Append(buffer, &isNull, sizeof(short));
									if(elt.type.archetype->name == "String")
									{
										String* str = (String*)elt.getDataAddress(cmp);
										size = str->size()*sizeof(std::string::value_type);
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, str->data(), size);
									}
									else if(elt.type.archetype->name == "vectorStr")
									{
										vectorStr* vstr = (vectorStr*)elt.getDataAddress(cmp);
										size = vstr->size();
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										for(auto& str : *vstr)
										{
											std::size_t strSize = str.size()*sizeof(std::string::value_type);
											ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
											ResourcesLoader::Append(buffer, str.data(), strSize);
										}
									}
									else
									{
										size = elt.type.archetype->memorySize;
										//store isNull
										//store field data
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, elt.getDataAddress(cmp), size);

									}
								}

						    }


						}
			            LambdaCmp(sub);
		            }

	            };
	            LambdaCmp(world);
				file.write(buffer.data(), buffer.size());
				SceneResource* scene = new SceneResource();
				scene->rawScene = buffer;
				scene->name = name;
				Engine::GetInstance()->resourceManager.AddResource(scene);


            }
            if (UI::MenuItem("Load Scene"))
            {
	            EditorInterface::selectedGO = nullptr;
            	json j;
            	std::ifstream file("test.SolidScene",std::ifstream::binary | std::ifstream::ate);



                //SCENE : implement load function here
	            if(!file.is_open())
	            {
	            	abort();
	            }
	            rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
	            std::uint64_t readPos = 0;
	            std::size_t cmpNameSize = 0;
	            std::ifstream::pos_type pos = file.tellg()  ;

	            std::vector<char>  buffer(pos);

	            file.seekg(0, std::ios::beg);
	            file.read(&buffer[0], pos);

	            std::size_t jsonSize = 0;
	            ResourcesLoader::ReadFromBuffer(buffer.data(), &jsonSize, sizeof(std::size_t),readPos);
	            std::string jsonStr;
	            jsonStr.resize(jsonSize / sizeof(std::string::value_type));
	            ResourcesLoader::ReadFromBuffer(buffer.data(), jsonStr.data(), jsonSize,readPos);
	            j = j.parse(jsonStr) ;

	            Engine* engine = Engine::GetInstance();
	            GameObject* world = engine->ecsManager.GetWorld();
	            for (auto it = world->childs.begin() ; it != world->childs.end();)
	            {
		            (*it)->RemoveCurrent();
		            it = world->childs.begin();
		            if(it == world->childs.end())
		            {
			            break;
		            }
	            }

	            std::function<void(json&,Entity)> Lambda = [&, engine](json& j,Entity e){
		            for(auto it = j.begin(); it != j.end(); ++it)
		            {
			            std::string key = (it).key();
			            if(key.find("GameObject") != std::string::npos)
			            {
				            GameObject* ent = engine->ecsManager.CreateEntity( it.value()["Name"], e);
				            Lambda(std::ref(it.value()), ent->GetEntity());
			            }
		            }
	            };
	            for(auto it = j["Scene"].begin(); it != j["Scene"].end(); ++it)
	            {
		            std::string key = (it).key();
		            if(key.find("GameObject") != std::string::npos)
		            {
			            std::string name = it.value()["Name"];
			            GameObject* ent = engine->ecsManager.CreateEntity(name);
			            Lambda(std::ref(it.value()), ent->GetEntity());
		            }
	            }
	            std::function<void(GameObject*)> AddAllComps = [&](GameObject* elt)
	            {
		            //get num of Childs
		            std::size_t childNum = 0;
		            ResourcesLoader::ReadFromBuffer(buffer.data(), &childNum, sizeof(std::size_t), readPos);

		            for (int games = 0; games < childNum; ++games)
		            {
			            GameObject* go = elt->childs.at(games);

		                //get num of comps
		                std::size_t cmpNum = 0;
		                ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNum, sizeof(std::size_t), readPos);


			            for (int j = 0; j < cmpNum; ++j)
			            {
				            //get Comp Class / str
				            ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t), readPos);
				            std::string className;
				            className.resize(cmpNameSize / sizeof(std::string::value_type));
				            ResourcesLoader::ReadFromBuffer(buffer.data(), className.data(), cmpNameSize, readPos);

				            //get Field Num
				            std::size_t FieldNum = 0;
				            ResourcesLoader::ReadFromBuffer(buffer.data(), &FieldNum, sizeof(std::size_t), readPos);


				            rfk::Class const *myClass = n->getClass(className);
				            Components *cmp = myClass->makeInstance<Components>();

				            if (className == "Transform")
				            {
					            Transform *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(Transform *) cmp);
					            for (int i = 0; i < FieldNum; ++i)
					            {
						            short isNull = -1;

						            //Get Comp FieldName
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
						                                            readPos);
						            std::string Name;
						            Name.resize(cmpNameSize / sizeof(std::string::value_type));
						            ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

						            //Get is Null
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
						            std::size_t memSize = 0;
						            //Get Field Data
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
						            if (isNull == 256)
						            {}
						            else
						            {}
						            char *buf = new char[memSize]();
						            ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);
						            const rfk::Field *f = t->getArchetype().getField(Name);
						            f->setData(t, ((void *) buf), memSize);
						            delete[] buf;

						            if (Name == "rotation")
						            {
							            t->SetRotation(t->GetRotation());
						            }
					            }

				            }
				            else if (className == "AudioSource")
				            {
					            AudioSource *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(AudioSource *) cmp);

					            //Get Comp FieldName
					            for (int i = 0; i < FieldNum; ++i)
					            {
						            short isNull = -1;

						            //Get Comp FieldName
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
						                                            readPos);
						            std::string Name;
						            Name.resize(cmpNameSize / sizeof(std::string::value_type));
						            ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

						            //Get IsNull
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
						            std::size_t memSize = 0;
						            //get Field Data
						            const rfk::Field *f = t->getArchetype().getField(Name);
						            if (f->type.archetype->name == "String")
						            {
							            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
							                                            readPos);
							            String *str = (String *) f->getDataAddress(t);
							            str->resize(memSize / sizeof(std::string::value_type));

							            ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
						            }
						            else
						            {

							            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
							                                            readPos);
							            char *buf = new char[memSize]();
							            ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

							            if (isNull == 256)
							            {
								            std::string s = std::string(buf, memSize);
								            f->setData(t, s);
							            }
							            else
							            {

								            f->setData(t, ((void *) buf), memSize);
							            }


							            delete[] buf;
						            }

					            }
					            t->Init();
				            }
				            else if (className == "MeshRenderer")
				            {
					            MeshRenderer *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(MeshRenderer *) cmp);

					            //Get Comp FieldName
					            for (int i = 0; i < FieldNum; ++i)
					            {
						            short isNull = -1;

						            //Get Comp FieldName
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
						                                            readPos);
						            std::string Name;
						            Name.resize(cmpNameSize / sizeof(std::string::value_type));
						            ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

						            //Get IsNull
						            ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
						            std::size_t memSize = 0;
						            //get Field Data
						            const rfk::Field *f = t->getArchetype().getField(Name);
						            if (f->type.archetype->name == "String")
						            {
							            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
							                                            readPos);
							            String *str = (String *) f->getDataAddress(t);
							            str->resize(memSize / sizeof(std::string::value_type));

							            ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
						            }
						            else if (f->type.archetype->name == "vectorStr")
						            {
							            std::size_t vecSize = 0;
							            ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
							                                            readPos);
							            vectorStr *vstr = (vectorStr *) f->getDataAddress(t);

							            for (int k = 0; k < vecSize; ++k)
							            {
								            String str;
								            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                            readPos);
								            str.resize(memSize);
								            ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos);

								            vstr->push_back(std::move(str));
							            }
						            }
						            else
						            {

							            ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
							                                            readPos);
							            char *buf = new char[memSize]();
							            ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

							            if (isNull == 256)
							            {
								            std::string s = std::string(buf, memSize);
								            f->setData(t, s);
							            }
							            else
							            {

								            f->setData(t, ((void *) buf), memSize);
							            }


							            delete[] buf;
						            }

					            }
					            t->Init();
				            }
			            }
			            AddAllComps(go);
		            }
	            };
	            AddAllComps(world);

            }
            if (UI::BeginMenu("Build"))
            {
                if (UI::MenuItem("Windows"))
                    Log::Send("Building for Windows", Log::ELogSeverity::ERROR);
	            if (UI::MenuItem("Create Cmake"))
	            {
	            	GameCompiler::GetInstance()->CreateCmake();
	            	Log::Send("Cmake Create", Log::ELogSeverity::ERROR);
	            }
	            if (UI::MenuItem("Compile"))
	            {
		            GameCompiler::GetInstance()->LaunchCompile();
		            Log::Send("Compiling", Log::ELogSeverity::ERROR);
	            }
                UI::MenuItem("Linux");
                UI::EndMenu();
            }
            UI::EndMenu();
        }
    }

    void EditorInterface::DrawMenuWindows()
    {
        if(UI::BeginMenu("Windows"))
        {
            DrawMenuModules();
            DrawMenuTools();
            UI::EndMenu();
        }

    }

    void EditorInterface::DrawMenuTools()
    {
        if (UI::BeginMenu("Tools"))
        {
            if (UI::MenuItem("Colors Settings", nullptr, &colorOpen))
                Log::Send("Opened Colors Settings", Log::ELogSeverity::WARNING);
            if (UI::MenuItem("Performance Overlay", nullptr, &perfOpen))
                Log::Send("Perf Overlay opened or closed", Log::ELogSeverity::CRITICAL);
            if (UI::MenuItem("Show UI Demo", nullptr, &demoOpen))
                Log::Send("Opened Show Demo UI", Log::ELogSeverity::DEBUG);

            UI::EndMenu();
        }
    }

    void EditorInterface::DrawMenuModules()
    {
        if (UI::BeginMenu("Modules"))
        {
            if (UI::MenuItem("Files", nullptr, &filesInterface.p_open))
                Log::Send("Opened/closed files module", Log::ELogSeverity::INFO);
            if (UI::MenuItem("Hierarchy Tree", nullptr, &hierarchyTreeInterface.p_open))
                Log::Send("Opened/closed hierarchy tree module", Log::ELogSeverity::INFO);
            if (UI::MenuItem("Inspector", nullptr, &inspectorInterface.p_open))
                Log::Send("Open/closed inspector module", Log::ELogSeverity::INFO);
            if (UI::MenuItem("Logs", nullptr, &logsInterface.p_open))
                Log::Send("Opened/closed logs module", Log::ELogSeverity::INFO);
            if (UI::MenuItem("Play", nullptr, &playInterface.p_open))
                Log::Send("Opened/closed play module", Log::ELogSeverity::INFO);
            if (UI::MenuItem("Scene", nullptr, &sceneInterface.p_open))
                Log::Send("Opened/closed scene module", Log::ELogSeverity::INFO);

            UI::EndMenu();
        }
    }




#pragma endregion
#pragma region Windows
    void EditorInterface::DrawMainFrame()
    {
        DrawMenuBar();

        ImGuiDockNodeFlags dockFlags = 0;
        dockFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
        UI::DockSpaceOverViewport(UI::GetMainViewport(), dockFlags);
    }

    void EditorInterface::AddInfoOverlay()
    {
        UI::SetNextWindowBgAlpha(0.4);
        UI::SetNextWindowSize(ImVec2(250, 150));
        UI::SetNextWindowViewport(UI::GetID(UI::GetMainViewport()));

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
        flags |= ImGuiWindowFlags_NoTitleBar;

        UI::Begin("Additional Informations", &perfOpen, flags);

        std::string fps = std::to_string(Time::Fps()) + " fps";
        UI::Text("%s", fps.c_str());

        Vec3 camPos = Editor::editorCamera.transform.GetPosition();
        std::string posSubText = std::to_string(camPos.x);
        std::string xCut = posSubText.substr(0, posSubText.find(".")+3);

        posSubText = std::to_string(camPos.y);
        std::string yCut = posSubText.substr(0, posSubText.find(".")+3);

        posSubText = std::to_string(camPos.z);
        std::string zCut = posSubText.substr(0, posSubText.find(".")+3);

        std::string camPosStr = "Camera position:\nx: " + xCut + " y: " + yCut + " z: " + zCut;
        UI::Text("%s", camPosStr.c_str());

        UI::End();

    }


    void EditorInterface::DrawChangeColors()
    {

        ImVec2 size((float) (window->GetWindowSize().x) / 3.f, (float) (window->GetWindowSize().y) / 1.5f);
        UI::SetNextWindowSize(size);
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoDocking;
        UI::Begin("colorsSettings", &colorOpen, flags);

        if (UI::CloseButton(UI::GetID("colorsSettings"), ImVec2(0, 0)))
            colorOpen = false;

        if (UI::Button("Dark Theme"))
            DarkTheme();
        UI::SameLine();
        if (UI::Button("Light Theme"))
            LightTheme();

        UI::ColorEdit4("##Text Preview", (float *) &editorStyle.Colors[0], ImGuiColorEditFlags_NoInputs);
        UI::SameLine();
        UI::DragFloat4("Text", (float *) &editorStyle.Colors[0], 0.005f, 0.f, 1.f);

        UI::ColorEdit4("##BG Preview", (float *) &editorStyle.Colors[2], ImGuiColorEditFlags_NoInputs);
        UI::SameLine();
        UI::DragFloat4("Background", (float *) &editorStyle.Colors[2], 0.005f, 0.f, 1.f);
        editorStyle.Colors[ImGuiCol_ChildBg] = editorStyle.Colors[2];

        UI::ColorEdit4("##Border Preview", (float *) &editorStyle.Colors[5], ImGuiColorEditFlags_NoInputs);
        UI::SameLine();
        UI::DragFloat4("Border", (float *) &editorStyle.Colors[5], 0.005f, 0.f, 1.f);

        UI::ColorEdit4("##Tab Unfocused Active Preview", (float *) &editorStyle.Colors[ImGuiCol_TabUnfocusedActive],
                       ImGuiColorEditFlags_NoInputs);
        UI::SameLine();
        UI::DragFloat4("Tab Unfocused Active",
                       (float *) &editorStyle.Colors[ImGuiCol_TabUnfocusedActive], 0.005f, 0.f, 1.f);

        UI::ColorEdit4("##Tab Unfocused Active", (float *) &editorStyle.Colors[ImGuiCol_TabActive],
                       ImGuiColorEditFlags_NoInputs);
        UI::SameLine();
        UI::DragFloat4("Tab Active",
                       (float *) &editorStyle.Colors[ImGuiCol_TabActive], 0.005f, 0.f, 1.f);


        UI::End();
    }

#pragma endregion
#pragma region Colors

    void EditorInterface::DarkTheme()
    {
        Log::Send("Changed to Dark theme", Log::ELogSeverity::INFO);

        editorStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
        editorStyle.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.f);
        editorStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

        editorStyle.Colors[ImGuiCol_DockingPreview] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);

        editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

        editorStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
        editorStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_Separator] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);


        editorStyle.Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
        editorStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        editorStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.03f, 0.03f, 0.03f, 1.f);
        editorStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

        editorStyle.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.f);
        editorStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.75f, 0.75f, 1.f);

        editorStyle.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        editorStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        editorStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_Header] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
        editorStyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        editorStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
        editorStyle.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
    }

    void EditorInterface::LightTheme()
    {
        Log::Send("Changed to Light theme", Log::ELogSeverity::INFO);

        editorStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.f);
        editorStyle.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
        editorStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.f);

        editorStyle.Colors[ImGuiCol_DockingPreview] = ImVec4(0.6, 0.6f, 0.6f, 1.f);

        editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
        editorStyle.Colors[ImGuiCol_Border] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);

        editorStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
        editorStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        editorStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_Separator] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
        editorStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
        editorStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);

        editorStyle.Colors[ImGuiCol_Tab] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
        editorStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.f);
        editorStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.9f, 0.9f, 0.9f, 1.f);
        editorStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.75f, 0.75f, 0.75f, 1.f);
        editorStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);

        editorStyle.Colors[ImGuiCol_Text] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
        editorStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

        editorStyle.Colors[ImGuiCol_Button] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
        editorStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);

        editorStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
        editorStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);

        editorStyle.Colors[ImGuiCol_Header] = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
        editorStyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.f);

        editorStyle.Colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
        editorStyle.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    }
}

#pragma endregion
