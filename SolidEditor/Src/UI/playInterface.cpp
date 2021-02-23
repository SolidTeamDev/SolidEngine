#include "UI/playInterface.hpp"
#include "Window/window.hpp"

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
        if (UI::IsWindowFocused() && UI::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            cursorInv = true;

        if (cursorInv)
        {
            ImVec2 pos = UI::GetWindowSize();
            SetCursorPos(pos.x, pos.y);
            UI::SetMouseCursor(ImGuiMouseCursor_None);
        }
        if (UI::IsKeyPressed(UI::GetKeyIndex(ImGuiKey_Enter)))
        {
            cursorInv = false;
            UI::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }

        UI::TextWrapped("Welcome on the Solid Play Scene!");
        UI::End();
    }
}
