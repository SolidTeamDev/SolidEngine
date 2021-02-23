#include "UI/playInterface.hpp"

#include <imgui.h>

namespace Solid
{
    void PlayInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));
        UI::Begin("Play", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        UI::TextWrapped("Welcome on the Solid Play Scene!");
        UI::End();
    }
}
