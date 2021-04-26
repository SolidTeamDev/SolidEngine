//
// Created by ryan1 on 21/04/2021.
//


#include "editor.hpp"
#include "launcher.hpp"
#include "imgui_stdlib.h"


using namespace Solid;

Solid::Launcher::Launcher()
{
	WindowParams windowParams
			{
					.title = "Solid Launcher",
					.windowSize = {1280,720}
			};

	RendererParams rendererParams
			{
					.rendererType = ERendererType::OpenGl45
			};
	engine = Engine::GetInstance();
	engine->InitEngineContext(windowParams, rendererParams);
	glfwSetWindowAttrib(engine->window->GetHandle(), GLFW_RESIZABLE, GLFW_FALSE);

	if(!engine->IsEngineContextInitialized())
		throw ThrowError("Engine not correctly initialized !",ESolidErrorCode::S_INIT_ERROR);

	const Window* window = engine->window;

}

Solid::Launcher::~Launcher()
{

}

void Solid::Launcher::Run()
{
	Window* window = engine->window;
	Renderer* renderer = engine->renderer;

	glfwSwapInterval(0);



	auto& io = UI::GetIO();
	auto& style = UI::GetStyle();
	Vec3 c = Vec3(29, 124, 139);
	style.Colors[2] = ImVec4(c.x/255.0, c.y/255.0, c.z/255.0,1.0);
	style.FramePadding.y =0;
	//io.ConfigFlags = io.ConfigFlags & ~ImGuiConfigFlags_ViewportsEnable;

	while (!glfwWindowShouldClose(window->GetHandle()) && !needClose)
	{
		glfwPollEvents();
		//TODO: Update engine task in engine
		engine->Update();
		engine->FixedUpdate();
		engine->LateUpdate();

		renderer->ClearColor({0,0,0,1});
		renderer->Clear(window->GetWindowSize());

		UpdateInterface();


		Time::Update();

		window->SwapBuffers();
	}
}

void Solid::Launcher::UpdateInterface()
{
	UIContext::BeginFrame();

	auto s = engine->window->GetWindowSize();


	auto& io = UI::GetIO();

	UI::SetNextWindowPos(UI::GetMainViewport()->Pos);
	auto& style = UI::GetStyle();

	UI::SetNextWindowSize({(float)s.x,(float)s.y});
	Vec2 buttonPos = Vec2(200, 100);
	Vec2 buttonSize = Vec2(100,100);
	UI::Begin("##Launcher", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		io.FontGlobalScale = 1.5;
		Vec2 size =Vec2{UI::CalcTextSize("New Project").x + style.FramePadding.x
			  , UI::CalcTextSize("New Project").y + style.FramePadding.y } + buttonSize;
		UI::SetCursorPos({buttonPos.x  - (size.x/2.0f),
					buttonPos.y- (size.y/2.0f)});
		if(UI::Button("New Project", {size.x,size.y}))
		{
			ImGui::OpenPopup("Create New Project");
		}
		UI::SetCursorPos({s.x - buttonPos.x  - ((size.x)/2.0f),
		                  buttonPos.y- (size.y/2.0f)});
		if(UI::Button("Open Project", {size.x,size.y}))
		{
			ImGui::OpenPopup("Open Project##2");
		}
		if(fileBrowser.showFileDialog("Create New Project", imgui_addons::ImGuiFileBrowser::DialogMode::SELECT))
		{
			std::cout << "create project at : "<< fileBrowser.selected_path << std::endl ;
			UI::OpenPopup("ProjectName");
			ProjName.clear();
		}
		if(UI::BeginPopup("ProjectName"))
		{
			UI::InputText("ProjectName", &ProjName);
			if(UI::Button("Create Project"))
			{
				if(!ProjName.empty())
				{
					std::filesystem::path p = fileBrowser.selected_path;
					std::filesystem::path p2 = fileBrowser.selected_path;
					p= p.append(ProjName);
					p2= p2.append(ProjName);
					if(std::filesystem::exists(p))
					{
						ErrorStr = "Error : Project with this name already exist";

						UI::OpenPopup("ERROR");
					}
					else
					{
						std::filesystem::create_directory(p);
						p = p.append("ProjectSettings.SolidProject");
						json j;
						j["Project"]["Name"] = ProjName;
						j["Project"]["Path"] = p2.string();
						j["Project"]["AssetFolder"] = "Assets";
						j["Project"]["SourcesFolder"] = "Code";
						j["Project"]["EngineIncludeFolder"] = "EngineInclude";
						std::ofstream file(p);
						auto p3 = p;
						p = p2;
						p.append("Assets");
						std::filesystem::create_directory(p);
						p = p2;
						p.append("Code");
						std::filesystem::create_directory(p);
						p = p2;
						p.append("EngineInclude");
						std::filesystem::create_directory(p);
						file << std::setw(4) << j;
						file.close();

						std::string launch = "start SolidEditor.exe -Project=" + p3.string();
						std::system(launch.c_str());
						needClose = true;
						UI::CloseCurrentPopup();
					}

				}
				else
				{
					ErrorStr = "Error : Project Name Is Empty";
					UI::OpenPopup("ERROR");
				}
			}
			UI::SameLine();
			if(UI::Button("Cancel"))
			{
				UI::CloseCurrentPopup();
			}
			if(UI::BeginPopupModal("ERROR", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
			{
				UI::Text(ErrorStr.c_str());
				if(UI::Button("Understood"))
				{
					UI::CloseCurrentPopup();
				}
				UI::EndPopup();
			}
			UI::EndPopup();
		}

		if(fileBrowser.showFileDialog("Open Project##2", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, {0,0},".SolidProject") )
		{
			std::cout << "open project at : "<< fileBrowser.selected_path << std::endl ;
			std::cout << "named : "<< fileBrowser.selected_fn << std::endl ;
			std::string launch = "start SolidEditor.exe -Project=" + fileBrowser.selected_path;
			std::system(launch.c_str());
			needClose = true;
		}

	}

	UI::End();



	UIContext::RenderFrame();
}