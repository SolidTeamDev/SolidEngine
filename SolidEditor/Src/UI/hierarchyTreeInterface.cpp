#include "UI/hierarchyTreeInterface.hpp"
#include "UI/editorInterface.hpp"
#include <imgui.h>
#include "ECS/Components/meshRenderer.hpp"


namespace Solid
{
    void HierarchyTreeInterface::Draw()
    {
        if(!p_open)
            return;

        Engine* engine = Engine::GetInstance();

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Hierarchy", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        if (UI::IsWindowHovered() && UI::IsAnyMouseDown())
            EditorInterface::selectedGO = nullptr;

        for(GameObject* g : engine->ecsManager.GetWorld()->childs)
        {
            DrawEntities(g,0);
        }
        DrawCreateObject();

        UI::End();
    }
}

void Solid::HierarchyTreeInterface::DrawCreateObject()
{
    Engine* engine = Engine::GetInstance();

    if(UI::BeginPopupContextWindow("createObject"))
    {
        if(UI::BeginMenu("New"))
        {
            if(UI::MenuItem("GameObject"))
            {
                Entity tmp = -1;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                engine->ecsManager.AddComponent(tmp,Transform());
                EditorInterface::selectedGO = engine->ecsManager.GetGameObjectFromEntity(tmp);

            }
            UI::Separator();
            if(UI::MenuItem("Cube"))
            {
                Entity tmp = -1;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                engine->ecsManager.AddComponent(tmp,Transform());
                engine->ecsManager.AddComponent(tmp,MeshRenderer(
                		engine->graphicsResourceMgr.GetMesh("cube.obj"))
                );
                EditorInterface::selectedGO = engine->ecsManager.GetGameObjectFromEntity(tmp);

            }
            if(UI::MenuItem("Sphere"))
            {

            }
            if(UI::MenuItem("Solid"))
            {
                Entity tmp = engine->ecsManager.CreateEntity();
                engine->ecsManager.AddComponent(tmp,Transform());
                engine->ecsManager.AddComponent(tmp,MeshRenderer(
		                engine->graphicsResourceMgr.GetMesh("solid.obj"))
                );
                EditorInterface::selectedGO = engine->ecsManager.GetGameObjectFromEntity(tmp);

            }
            UI::EndMenu();
        }

        UI::EndPopup();
    }
}

void Solid::HierarchyTreeInterface::DrawEntities(GameObject* child, unsigned int it)
{


    ImVec2 pos = UI::GetCursorPos();
    pos.x += 20.f * (float)it;
    UI::SetCursorPos(pos);

    DrawEntity(child);


    for(GameObject* child : child->childs)
    {
        DrawEntities(child, it+1);
    }

}

void Solid::HierarchyTreeInterface::DrawEntity(GameObject* child)
{
    ImVec4 colButton = UI::GetStyleColorVec4(ImGuiCol_Button);
    if (child == EditorInterface::selectedGO)
        colButton.w = 0.5f;
    else
        colButton.w = 0.0f;

    UI::PushStyleColor(ImGuiCol_Button, colButton);
    UI::PushStyleColor(ImGuiCol_ButtonHovered, colButton);

    if(UI::SmallButton((child->name + "##" +std::to_string(child->GetEntity())).c_str())
                        || (UI::IsAnyMouseDown() && UI::IsItemHovered()))
        EditorInterface::selectedGO = child;

    if(UI::IsItemHovered() && UI::IsMouseDragging(ImGuiMouseButton_Left) && child == EditorInterface::selectedGO)
        EditorInterface::draggingEnt = true;

    if(UI::IsItemHovered() && EditorInterface::draggingEnt && !UI::IsAnyMouseDown())
    {
        EditorInterface::draggingEnt = false;
        Log::Send("Changed " + EditorInterface::selectedGO->name +
                             "'s parent to " + child->name, Log::ELogSeverity::DEBUG);

		if(child != EditorInterface::selectedGO)
            EditorInterface::selectedGO->ReParentCurrent(child);

    }

    UI::PopStyleColor(2);
}

