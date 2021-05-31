
#ifdef __unix__
#define OS_WIN 0

#elif defined(_WIN32) || defined(WIN32)
#define OS_WIN 1
#include <windows.h>
#endif


#include "UI/editorInterface.hpp"

#include "UI/solidUI.hpp"
#include "Time/time.hpp"
#include "editor.hpp"

#include <imgui_internal.h>
#include <string>
#include <sstream>
#include "UI/winUI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/scriptList.hpp"
#include "imgui_stdlib.h"




Solid::GameObject* Solid::EditorInterface::selectedGO = nullptr;
bool               Solid::EditorInterface::draggingEnt = false;



namespace Solid {
    EditorInterface::EditorInterface() :
            editorStyle(UI::GetStyle())
    {
        window = nullptr;
        renderer = nullptr;
        DarkTheme();
        std::function<void(Resource*)> callback = std::bind(&EditorInterface::LoadSceneCall, this, std::placeholders::_1);
        Engine::GetInstance()->AddLoadedSceneCallback(callback);
    }

    EditorInterface::EditorInterface(Window *_window, Renderer* _renderer) :
            editorStyle(UI::GetStyle())
    {
        window = _window;
        renderer = _renderer;
        DarkTheme();
	    std::function<void(Resource*)> callback = std::bind(&EditorInterface::LoadSceneCall, this, std::placeholders::_1);
	    Engine::GetInstance()->AddLoadedSceneCallback(callback);
    }

    void EditorInterface::Update()
    {
        UIContext::BeginFrame();

        DrawMainFrame();
	    if(openScenePopup)
	    {
		    openScenePopup =false;
		    UI::OpenPopup("OpenScene");
		    choice = currentScenePtr;
	    }
	    if(saveScenePopup)
	    {
		    saveScenePopup =false;
		    str = "";
		    UI::OpenPopup("SaveScene");
	    }
	    if(UI::BeginPopup("OpenScene"))
	    {
		    {

			    const char* sceneName = choice == nullptr ? "UNTITLED" : choice->name.c_str();

			    UI::Text("Scene  ");UI::SameLine();

			    bool combo =UI::BeginCombo("##Scene", sceneName);
			    if(UI::BeginDragDropTarget())
			    {

				    const ImGuiPayload* drop=UI::AcceptDragDropPayload("Scene");
				    if(drop != nullptr)
				    {
					    Resource* r = *((Resource**)drop->Data);
					    currentScenePtr = (SceneResource*)r;
					    currentOpenedScene = r->name;
					    currentScenePath = ResourcesLoader::SolidPath;
					    for (int i = 1; i < r->path.size(); ++i)
					    {
						    currentScenePath.append(r->path[i]);
					    }
					    Engine::GetInstance()->LoadScene(r->name.c_str());
						UI::CloseCurrentPopup();
				    }
				    UI::EndDragDropTarget();
			    }
			    if(combo)
			    {
				    auto sceneList = Engine::GetInstance()->resourceManager.GetResourcesVecByType(EResourceType::Scene);

				    for(auto scenes : *sceneList)
				    {
					    bool selected = (sceneName == scenes.second->name);
					    if(UI::Selectable(scenes.second->name.c_str(), selected))
					    {
						   choice = (SceneResource*)scenes.second;
					    }
					    if(selected)
						    UI::SetItemDefaultFocus();
				    }

				    UI::EndCombo();
			    }
				if(UI::Button("Load"))
				{
					currentScenePtr = Engine::GetInstance()->resourceManager.GetSceneByName(choice->name.c_str());
					currentOpenedScene = currentScenePtr->name;
					currentScenePath = ResourcesLoader::SolidPath;
					for (int i = 1; i < currentScenePtr->path.size(); ++i)
					{
						currentScenePath.append(currentScenePtr->path[i]);
					}
					Engine::GetInstance()->LoadScene(choice->name.c_str());

					UI::CloseCurrentPopup();
				}
		    }
		    UI::EndPopup();
	    }
	    if(UI::BeginPopup("SaveScene"))
	    {

		    UI::Text("Scene Name");
		    UI::SameLine();
		    UI::InputText("##sceneName", &str);
		    if(UI::Button("Save"))
		    {
		    	if(!str.empty())
			    {
				    std::size_t dotIndex = str.find_last_of('.');
				    if (dotIndex != std::string::npos)
				    {
					    str.erase(dotIndex);
				    }
				    str += ".SolidScene";
				    fs::path p = ResourcesLoader::SolidPath;
				    std::deque<std::string> path;
				    filePathData *node = filesInterface.currentFolder;
				    while (node->parent != nullptr)
				    {
					    path.push_front(node->folderName);
					    node = node->parent;
				    }
				    for (auto &elt : path)
				    {
					    p.append(elt);
				    }
				    p.append(str);
				    Engine::GetInstance()->SaveScene(p);
				    currentOpenedScene = p.filename().string();
				    currentScenePath = p;
				    UI::CloseCurrentPopup();
			    }
		    }
		    UI::EndPopup();
	    }


        filesInterface.Draw();
        inspectorInterface.Draw();
        hierarchyTreeInterface.Draw();
        playInterface.Draw();
        sceneInterface.Draw();
        logsInterface.Draw();
        buttonInterface.Draw();

        if (colorOpen)
            DrawChangeColors();
        if (perfOpen)
            AddInfoOverlay();
        if (demoOpen)
            UI::ShowDemoWindow();


        UIContext::RenderFrame();
        renderer->UpdateFramebuffer(sceneInterface.getSceneFrameBuffer());
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
	        if (UI::MenuItem("Save Project"))
	        {

	        }
            if (UI::MenuItem("Save Scene"))
            {
				if(currentOpenedScene == "Untitled")
				{
					saveScenePopup =true;
				}
				else
				{
					fs::path p = currentScenePath;
					p.append(currentOpenedScene);
					Engine::GetInstance()->SaveScene(p);
				}


            }
	        if (UI::MenuItem("Save Scene At"))
	        {

		        saveScenePopup = true;
	        }
            if (UI::MenuItem("Load Scene"))
            {
	            EditorInterface::selectedGO = nullptr;
	            openScenePopup = true;

            }
	        if (UI::BeginMenu("Build"))
            {

	            if (UI::MenuItem("Windows"))
	            {
		            GameCompiler::GetInstance()->Build();
		            Log::Send("Building for Windows", Log::ELogSeverity::ERROR);
	            }
	            if (UI::MenuItem("Update VC compiler"))
	            {
		            GameCompiler::GetInstance()->updateVCPath();
	            }
	            if (UI::MenuItem("Create Cmake"))
	            {
	            	GameCompiler::GetInstance()->CreateCmake();
	            	Log::Send("Cmake Create", Log::ELogSeverity::ERROR);
	            }
	            if (UI::MenuItem("Reload CMAKE"))
	            {
		            GameCompiler::GetInstance()->ReloadCmake();
		            Log::Send("Reloading Cmake", Log::ELogSeverity::ERROR);
	            }


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

        /*Vec3 camPos = Editor::editorCamera.transform.GetPosition();
        std::string posSubText = std::to_string(camPos.x);
        std::string xCut = posSubText.substr(0, posSubText.find(".")+3);

        posSubText = std::to_string(camPos.y);
        std::string yCut = posSubText.substr(0, posSubText.find(".")+3);

        posSubText = std::to_string(camPos.z);
        std::string zCut = posSubText.substr(0, posSubText.find(".")+3);

        std::string camPosStr = "Camera position:\nx: " + xCut + " y: " + yCut + " z: " + zCut;
        UI::Text("%s", camPosStr.c_str());*/

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

	void EditorInterface::LoadSceneCall(Resource* scene)
	{
		currentScenePtr = (SceneResource*)scene;
		currentOpenedScene = currentScenePtr->name;
		currentScenePath = ResourcesLoader::SolidPath;
		for (int i = 1; i < currentScenePtr->path.size(); ++i)
		{
			currentScenePath.append(currentScenePtr->path[i]);
		}
	}
}

#pragma endregion
