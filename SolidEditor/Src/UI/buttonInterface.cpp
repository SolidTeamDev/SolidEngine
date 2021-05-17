//
// Created by ryan1 on 17/05/2021.
//

#include "UI/buttonInterface.hpp"
#include <string>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <ECS/Components/scriptList.hpp>
#include "Core/engine.hpp"
#include "editor.hpp"
#include "imgui_stdlib.h"

using namespace Solid;
namespace fs = std::filesystem;


ButtonInterface::ButtonInterface()
{
	fs::path EditorAssets = fs::current_path();
	EditorAssets.append("EditorAssets");
	ResourcesLoader loader;
	{
		ResourcePtrWrapper wrap{.r=nullptr};
		loader.LoadRessourceNoAdd( EditorAssets.string() + "/Compile.png", wrap);
		if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
		{
			editorImage.emplace("Compile", (ImageResource*)wrap.r);
			std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
			if(Tex != nullptr)
			{
				editorTex.emplace("Compile", Tex);
			}
		}
	}
	{
		ResourcePtrWrapper wrap;
		loader.LoadRessourceNoAdd( EditorAssets.string() + "/Play.png", wrap);
		if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
		{
			editorImage.emplace("Play", (ImageResource*)wrap.r);
			std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
			if(Tex != nullptr)
			{
				editorTex.emplace("Play", Tex);
			}
		}
	}
	{
		ResourcePtrWrapper wrap;
		loader.LoadRessourceNoAdd( EditorAssets.string() + "/Pause.png", wrap);
		if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
		{
			editorImage.emplace("Pause", (ImageResource*)wrap.r);
			std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
			if(Tex != nullptr)
			{
				editorTex.emplace("Pause", Tex);
			}
		}
	}
	{
		ResourcePtrWrapper wrap;
		loader.LoadRessourceNoAdd( EditorAssets.string() + "/Stop.png", wrap);
		if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
		{
			editorImage.emplace("Stop", (ImageResource*)wrap.r);
			std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
			if(Tex != nullptr)
			{
				editorTex.emplace("Stop", Tex);
			}
		}

	}



}

void ButtonInterface::Draw()
{
	UI::Begin("Buttons", nullptr,
	          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoTitleBar);

	ImVec2 bS = ImVec2(40 ,40);
	if(UI::ImageButton((ImTextureID)editorTex["Play"]->texId, bS))
		Editor::Play();

	UI::SameLine();
	if(UI::ImageButton((ImTextureID)editorTex["Pause"]->texId, bS))
		Editor::Pause();

	UI::SameLine();

	if(UI::ImageButton((ImTextureID)editorTex["Stop"]->texId, bS))
		Editor::Stop();

	UI::SameLine();

	if (UI::ImageButton((ImTextureID)editorTex["Compile"]->texId, bS))
	{
		compile = true;
		GameCompiler::GetInstance()->ReloadCmake();
		auto compiler = GameCompiler::GetInstance();
		auto scriptListArray =Engine::GetInstance()->ecsManager.GetCompArray<ScriptList>();
		std::array<ScriptList, MAX_ENTITIES>& array = scriptListArray->GetArray();
		std::unordered_map<Entity, size_t>& idArray = scriptListArray->GetIndexesArray();
		std::unordered_map<size_t , Entity>& entArray = scriptListArray->GetEntitiesArray();
		std::vector<CompData> compsSave;
		for(auto& elt : idArray)
		{
			ScriptList scriptListToSave =array[elt.second];
			std::vector<Script*> allS = scriptListToSave.GetAllScripts();
			int i = 0;
			for(Script* scriptToSave : allS)
			{
				CompData compD;
				compD.compName = scriptToSave->getArchetype().name;
				compD.go = scriptToSave->gameObject;
				for(auto& field : scriptToSave->getArchetype().fields)
				{
					FieldData fieldD;
					fieldD.fName= field.name;
					void* fData =field.getDataAddress(scriptToSave);
					uint fSize = field.type.archetype->memorySize;
					fieldD.fData.resize(fSize);
					std::memcpy(fieldD.fData.data(), fData, fSize);
					compD.fields.push_back(std::move(fieldD));
				}
				delete allS[i];
				allS[i] = nullptr;
				compD.entityCompIndex = elt.second;
				compD.CompListIndex = i;
				compsSave.push_back(std::move(compD));
				++i;
			}

		}
		if(compiler->hGetProcIDDLL)
		{
			FreeLibrary(compiler->hGetProcIDDLL);
			compiler->hGetProcIDDLL = nullptr;
			compiler->entryPoint = nullptr;
			compiler->getClass = nullptr;
			compiler->getNamespace = nullptr;
		}
		fs::path DLLPath = GameCompiler::GetInstance()->DllPath;
		fs::path TmpDir = fs::current_path();
		TmpDir.append("Temp");
		fs::path TempDLL = TmpDir;
		TempDLL.append(GameCompiler::GetInstance()->ProjectName+"_Tmp.dll");
		compileResult = GameCompiler::GetInstance()->LaunchCompile();
		if(!compileResult)
		{

			if(!fs::exists(TmpDir))
			{
				//No Temp / First Gen ?
			}
			else
			{

				if(!fs::exists(TempDLL))
				{
					//No Temp / First Gen ?
				}
				else
				{
					std::ifstream in (TempDLL.string(), std::fstream::binary);
					std::ofstream out (DLLPath.string(), std::fstream::binary | std::fstream::trunc);
					out << in.rdbuf();
					in.close();
					out.close();
				}
			}


		}
		else
		{
			if(!fs::exists(TmpDir))
				fs::create_directory(TmpDir);
			std::ifstream in (DLLPath.string(), std::fstream::binary);
			std::ofstream out (TempDLL.string(), std::fstream::binary | std::fstream::trunc);
			out << in.rdbuf();
			in.close();
			out.close();
		}
		compiler->hGetProcIDDLL = LoadLibrary(TempDLL.string().c_str());
		if(compiler->hGetProcIDDLL == nullptr)
		{
			Log::Send("LIB LOAD FAILED", Log::ELogSeverity::ERROR);
			compiler->entryPoint = nullptr;
			compiler->getClass = nullptr;
			compiler->getNamespace = nullptr;
		}
		else
		{
			compiler->entryPoint = (f_Entry)GetProcAddress(compiler->hGetProcIDDLL, "Entry");
			compiler->getClass = (f_GetClass)GetProcAddress(compiler->hGetProcIDDLL, "GetClass");
			compiler->getNamespace = (f_GetNamespace)GetProcAddress(compiler->hGetProcIDDLL, "GetNamespace");
			if(compiler->entryPoint == nullptr)
			{
				Log::Send("entryPoint = nullptr", Log::ELogSeverity::ERROR);

			}
			if(compiler->getNamespace != nullptr)
			{

				for(auto& elt : compsSave)
				{
					const rfk::Class* c= compiler->getNamespace("Solid")->getClass(elt.compName);
					if(c == nullptr)
					{

						///OUCH
					}
					else
					{
						Script* newComp= c->makeInstance<Script>();
						newComp->gameObject = elt.go;
						for(auto& field : elt.fields)
						{
							const rfk::Field* f= newComp->getArchetype().getField(field.fName);
							if(f != nullptr)
								f->setData(newComp, field.fData.data(), field.fData.size());
						}
						array[elt.entityCompIndex].GetAllScripts()[elt.CompListIndex] = newComp;
					}

				}
				for(auto& elt : idArray)
				{
					ScriptList scriptListToClean =array[elt.second];
					scriptListToClean.CleanAllNullptr();
				}


			}
			else
			{
				Log::Send("getNamespace = nullptr", Log::ELogSeverity::ERROR);
			}


		}


	}
	int off = 20;

	if(compile)
	{
		UI::OpenPopup("Compile");
		compile = false;
		popOpen = true;
	}
	ImVec2 pos = ImVec2(UI::GetMainViewport()->Pos.x + UI::GetMainViewport()->Size.x -200,
					 UI::GetMainViewport()->Pos.y + UI::GetMainViewport()->Size.y-100);
	UI::SetNextWindowPos(pos);
	UI::SetNextWindowSize(ImVec2(190, 90));
	if(UI::BeginPopup("Compile"))
	{
		if(compileResult)
		{
			UI::SetCursorPos(ImVec2(40,40));
			UI::Text("Compile SUCCESS");
		}
		else
		{
			UI::SetCursorPos(ImVec2(40,40));
			UI::Text("Compile FAILURE");
		}
		UI::EndPopup();

	}

	UI::SetCursorPos(ImVec2(15,UI::GetCursorPosY()));
	UI::Text("Play");
	UI::SameLine();
	UI::SetCursorPos(ImVec2(68,UI::GetCursorPosY()));

	UI::Text("Pause");
	UI::SameLine();
	UI::SetCursorPos(ImVec2(127,UI::GetCursorPosY()));

	UI::Text("Stop");
	UI::SameLine();
	UI::SetCursorPos(ImVec2(174,UI::GetCursorPosY()));
	UI::Text("Compile");
	UI::SameLine();

	UI::End();
}
