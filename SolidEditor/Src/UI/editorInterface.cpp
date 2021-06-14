
#ifdef __unix__
#define OS_WIN 0

#elif defined(_WIN32) || defined(WIN32)
#define OS_WIN 1
#include <windows.h>
#endif
#include "fmod.hpp"

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
	    std::function<void(Entity, void*)> callback2 = std::bind(&EditorInterface::DestroyedSelectedGO, this, std::placeholders::_1, std::placeholders::_2);
        Engine::GetInstance()->ecsManager.AddDestroyedEntityCallback(callback2);
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
        if (computeDemo)
        	AddComputeOverlay();
        if(openBuildMenu)
        	DrawBuildMenu();


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
	        if (UI::MenuItem("New Scene"))
	        {
	        	currentOpenedScene = "Untitled";
	        	currentScenePtr = nullptr;
	        	currentScenePath = "";
		        Engine* engine = Engine::GetInstance();
		        GameObject* world = engine->ecsManager.GetWorld();
		        for (auto it = world->childs.begin() ; it != world->childs.end();)
		        {
			        engine->ecsManager.DestroyEntity((*it)->GetEntity());
			        //(*it)->RemoveCurrent();
			        it = world->childs.begin();
			        if(it == world->childs.end())
			        {
				        break;
			        }
		        }
		        EditorInterface::selectedGO = nullptr;
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
	        if (UI::BeginMenu("Package"))
            {

	            if (UI::MenuItem("Windows"))
	            {
		           openBuildMenu =true;
	            }
	            if (UI::MenuItem("Reload CMAKE"))
	            {
		            GameCompiler::GetInstance()->ReloadCmake();
	            }

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
	        if (UI::MenuItem("Compute Shader Demo", nullptr, &computeDemo))
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

        /*Vec3 camPos = Editor::editorCamera.transform.GetLocalPosition();
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

	void EditorInterface::AddComputeOverlay()
	{
		//UI::SetNextWindowBgAlpha(0.4);
		UI::SetNextWindowSize(ImVec2(600, 600));
		//UI::SetNextWindowViewport(UI::GetID(UI::GetMainViewport()));

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoResize;// | ImGuiWindowFlags_NoCollapse;
		//flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
		//flags |= ImGuiWindowFlags_NoTitleBar;

		UI::Begin("Compute Shader Demo", &computeDemo, flags);

		UI::Text("Compute Shader Demo");
		UI::Text("if you don't see anything the compute shader is nullptr");
		UI::Separator();

		const char* meshName = currentComputeShader == nullptr ? "" : currentComputeShader->name.c_str();
		bool combo =UI::BeginCombo("##Mesh", meshName);
		Engine* engine = Engine::GetInstance();
		if(UI::BeginDragDropTarget())
		{

			const ImGuiPayload* drop=UI::AcceptDragDropPayload("Compute");
			if(drop != nullptr)
			{
				Resource* r = *((Resource**)drop->Data);
				currentComputeShader = engine->graphicsResourceMgr.GetCompute(r->name.c_str());

			}
			UI::EndDragDropTarget();
		}
		if(combo)
		{
			auto* meshList = engine->resourceManager.GetResourcesVecByType<ComputeShaderResource>();

			for(auto mesh : *meshList)
			{
				bool selected = (meshName == mesh.second->name);
				if(UI::Selectable(mesh.second->name.c_str(), selected))
				{
					currentComputeShader =  engine->graphicsResourceMgr.GetCompute(mesh.second->name.c_str());

				}
				if(selected)
					UI::SetItemDefaultFocus();
			}

			UI::EndCombo();
		}
		if(currentComputeShader)
		{
			if(UI::Button("Edit Compute shader"))
			{
				codeEditor.isCodeEditorOpen = true;
				codeEditor.imCodeEditor.SetText(currentComputeShader->GetComputeSource());
				codeEditor.codeType = CodeEditor::ECodeType::COMPUTE;
			}
			if(codeEditor.isCodeEditorOpen)
			{
				auto cpos = codeEditor.imCodeEditor.GetCursorPosition();
				UI::SetNextWindowSize(ImVec2(800,600),ImGuiCond_Once);
				UI::Begin("Edit shader##Window", &codeEditor.isCodeEditorOpen,ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
				ImGuiIO& io = ImGui::GetIO();
				auto shift = io.KeyShift;
				auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
				auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;




				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Save","Ctrl-S"))
						{
							currentComputeShader->SetComputeSource(codeEditor.imCodeEditor.GetText());
							currentComputeShader->ReloadShader();
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
				            codeEditor.imCodeEditor.GetLanguageDefinition().mName.c_str(), currentComputeShader->name.c_str());

				codeEditor.imCodeEditor.Render("Edit shader",UI::GetContentRegionAvail(), false);
				if (codeEditor.imCodeEditor.WantSave())
				{
					currentComputeShader->SetComputeSource(codeEditor.imCodeEditor.GetText());
					currentComputeShader->ReloadShader();
				}
				UI::End();
			}
			UI::Separator();


			currentComputeShader->InitTex({128, 128});
			currentComputeShader->BindShader();
			uint Result = currentComputeShader->Dispatch();
			UI::Image((ImTextureID) (size_t) Result, {512, 512});
			currentComputeShader->UnbindShader();
		}


		UI::End();
	}

	void EditorInterface::DestroyedSelectedGO(Entity _id, void* _ptr)
	{
		if(_ptr == EditorInterface::selectedGO)
			EditorInterface::selectedGO = nullptr;
	}

	void EditorInterface::DrawBuildMenu()
	{
		UI::SetNextWindowSize(ImVec2(600, 600));
		//UI::SetNextWindowViewport(UI::GetID(UI::GetMainViewport()));

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoResize;// | ImGuiWindowFlags_NoCollapse;
		//flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
		//flags |= ImGuiWindowFlags_NoTitleBar;

		UI::Begin("Packaging Project", &openBuildMenu, flags);
		Engine* engine = Engine::GetInstance();
		std::string sceneName = GameCompiler::GetInstance()->MainScene;
		bool combo =UI::BeginCombo("##SCENE", sceneName.c_str());
		if(combo)
		{
			auto* sceneList = engine->resourceManager.GetResourcesVecByType<SceneResource>();

			for(auto& scene : *sceneList)
			{
				bool selected = (sceneName == scene.second->name);
				if(UI::Selectable(scene.second->name.c_str(), selected))
				{
					GameCompiler::GetInstance()->MainScene = scene.second->name;
				}
				if(selected)
					UI::SetItemDefaultFocus();
			}

			UI::EndCombo();
		}

		if(UI::Button("Begin Build For Windows"))
		{
			SceneResource* scene =Engine::GetInstance()->resourceManager.GetSceneByName(GameCompiler::GetInstance()->MainScene.c_str());
			if(scene != nullptr)
			{
				GameCompiler::GetInstance()->Build();
				Log::Send("Building for Windows", Log::ELogSeverity::ERROR);
			}
			else
			{
				Log::Send("BUILD FAILED : NO MAIN SCENE CHOSEN OR SCENE NAME IS INVALID", Log::ELogSeverity::ERROR);
			}
		}

		UI::End();
	}
}

#pragma endregion
