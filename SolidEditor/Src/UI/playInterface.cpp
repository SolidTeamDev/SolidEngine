#include "UI/playInterface.hpp"
#include "Window/window.hpp"

#include <imgui.h>

namespace Solid
{
    PlayInterface::PlayInterface()
    {
        engine = Engine::GetInstance();
    }

    void PlayInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));
        UI::Begin("Play", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	    std::function<Vec2i(ImVec2)> converti = [](ImVec2 v) -> Vec2i{ return {(int)v.x,(int)v.y};};

        ImVec2 windowSize = UI::GetContentRegionAvail();
		ImVec2 winPos = ImVec2(UI::GetWindowPos().x,UI::GetWindowPos().y +UI::GetCursorPos().y);
	    engine->PlayBuffer.size = (converti(windowSize).x <= 0 && converti(windowSize).y <= 0) ? engine->window->GetWindowSize() : converti(windowSize);
	    engine->PlayBuffer.pos = (converti(winPos).x <= 0 && converti(winPos).y <= 0) ? Vec2i{0,0} : converti(winPos);

	    engine->RenderToBuffer({(int) windowSize.x, (int) windowSize.y}, true);
	    UI::Image((ImTextureID)(size_t)engine->PlayBuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});

        UI::End();
    }
}
