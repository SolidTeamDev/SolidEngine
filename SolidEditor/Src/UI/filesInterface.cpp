#include <string>
#include "UI/filesInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/engine.hpp"
#include "imgui_stdlib.h"
#include "GameCompiler/gameCompiler.hpp"
namespace Solid
{
    void FilesInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Files", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::TextWrapped("Welcome on the Solid files explorer!");
        std::vector<ResourcesPathData> data = Engine::GetInstance()->resourceManager.GetAllResourcesPath();
        counter -= Time::DeltaTime();
        if(counter <= 0.0)
        {
        	counter = 2.0;
        	root.childPaths.clear();
        	root.fileNames.clear();
	        for(auto& elt : data)
	        {
		        if(elt.rPath.size() == 1)
		        {
			        root.fileNames.push_back(elt.RName);
			        continue;
		        }
		        if(elt.rPath.empty())
			        continue;
		        filePathData* node = &root;
		        for (int i = 1; i < elt.rPath.size(); ++i)
		        {
			        if(node->childPaths.contains(elt.rPath[i]))
			        {
				        node = &(node->childPaths[elt.rPath[i]]);
				        continue;
			        }
			        else
			        {
				        node->childPaths.emplace(elt.rPath[i], filePathData{.folderName=elt.rPath[i], .parent=node});
				        node = &(node->childPaths[elt.rPath[i]]);
				        continue;
			        }
		        }
		        node->fileNames.push_back(elt.RName);
	        }
        }
	    {
	    	if(currentFolder->parent != nullptr && UI::Button("..##previous"))
		    {
	    		currentFolder = currentFolder->parent;
		    }
		    for(auto& elt : currentFolder->childPaths)
		    {
		        if(UI::Button(elt.first.c_str()))
		        {
		        	currentFolder = &elt.second;
		        }
		    }
		    for(auto& elt : currentFolder->fileNames)
		    {
			    UI::Button(elt.c_str());
		    }
	    }

        DrawCreateFile();
	    DrawMatNamePopup();
	    UI::End();



    }

    void FilesInterface::DrawCreateFile()
    {

        if(UI::BeginPopupContextWindow("createFile"))
        {
            if(UI::BeginMenu("New"))
            {

	            if(UI::MenuItem("Create New Script"))
                {
	            	open = true;
                }

	            if (UI::MenuItem("Create Material"))
	            {

	            	matNamePopup = true;
	            	matNamestr = "";


	            }
                UI::EndMenu();
            }

            UI::EndPopup();
        }
        if(open)
        {
	        open = false;
	        fName = "";
        	UI::OpenPopup("FName");
        }

        if(UI::BeginPopup("FName"))
        {
	        UI::InputText("File Name",&fName);
	        if(UI::Button("Create"))
	        {
		        GameCompiler* Compiler = GameCompiler::GetInstance();
		        fs::path p = Compiler->srcPath.string() +"/Include/" + fName + ".hpp";
		        if(fName == "" || fs::exists(p))
		        {
					//error
		        }
		        else
		        	Compiler->CreateScript(fName);
	        }
        	UI::EndPopup();
        }
    }

	void FilesInterface::DrawMatNamePopup()
	{
    	if(matNamePopup)
	    {
    		UI::OpenPopup("MatNamePopUp");
	        matNamePopup = false;
	    }

		if (UI::BeginPopup("MatNamePopUp"))
		{
			UI::Text("Material Name :");
			UI::SameLine();
			UI::InputText("##MatName", &matNamestr);
			if(UI::Button("Create new Material"))
			{
				UI::ShowDemoWindow();
				Engine* engine = Engine::GetInstance();
				MaterialResource* mat =engine->resourceManager.CreateMaterial(matNamestr.c_str());
				mat->SetShader(engine->graphicsResourceMgr.GetShader("DefaultShader"));
				UI::CloseCurrentPopup();
			}
			UI::EndPopup();
		}

	}

	FilesInterface::FilesInterface()
	{
		root.folderName = "\\Assets\\";
	}
}
