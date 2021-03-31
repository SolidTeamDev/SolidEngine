#pragma once

#include <imgui.h>

namespace Solid
{
    class SceneInterface
    {
    public:
        SceneInterface()  = default;
        ~SceneInterface() = default;

        void Draw();
        void DrawMenu();

        bool p_open = true;
	    ImDrawList* GridList = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
	    void* gizmoWin = nullptr;
    };
}
