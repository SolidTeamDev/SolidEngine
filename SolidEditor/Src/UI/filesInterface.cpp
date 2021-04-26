#include <string>
#include "UI/filesInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/engine.hpp"
#include "imgui_stdlib.h"

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
                if(UI::MenuItem("Create .hpp"))
                {

                }
                if(UI::MenuItem("Create .cpp"))
                {

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
