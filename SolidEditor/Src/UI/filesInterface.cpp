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
        UI::End();

    }
}
