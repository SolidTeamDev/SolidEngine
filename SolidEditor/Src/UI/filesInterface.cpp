#include "UI/filesInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Solid
{
    void FilesInterface::Draw()
    {
        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Files", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::TextWrapped("Welcome on the Solid files explorer!");
        DrawCreateFile();
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
                UI::EndMenu();
            }

            UI::EndPopup();
        }
    }
}
