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
				mat->shader = engine->graphicsResourceMgr.GetShader("DefaultShader");
				UI::CloseCurrentPopup();
			}
			UI::EndPopup();
		}

	}
}
