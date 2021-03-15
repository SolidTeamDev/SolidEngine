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

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Hierarchy", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        if (UI::IsWindowHovered() && UI::IsAnyMouseDown())
            EditorInterface::selectedGO = nullptr;


        DrawEntities();

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
                engine->ecsManager.AddComponent(tmp,MeshRenderer{
                        .mesh   = engine->graphicsResourceMgr.GetMesh("cube.obj"),
                        .shader = engine->graphicsResourceMgr.GetShader("ZShader")

                });
                EditorInterface::selectedGO = engine->ecsManager.GetGameObjectFromEntity(tmp);

            }
            if(UI::MenuItem("Sphere"))
            {

            }
            if(UI::MenuItem("Solid"))
            {
                Entity tmp = engine->ecsManager.CreateEntity();
                engine->ecsManager.AddComponent(tmp,Transform());
                engine->ecsManager.AddComponent(tmp,MeshRenderer{
                        .mesh   = engine->graphicsResourceMgr.GetMesh("solid.obj"),
                        .shader = engine->graphicsResourceMgr.GetShader("ZShader")
                });
                EditorInterface::selectedGO = engine->ecsManager.GetGameObjectFromEntity(tmp);

            }
            UI::EndMenu();
        }

        UI::EndPopup();
    }
}

void Solid::HierarchyTreeInterface::DrawEntities()
{
    //UI::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 1.f));
    //UI::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    //UI::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.f));

    Engine* engine = Engine::GetInstance();
    for(GameObject* g : engine->ecsManager.GetWorld()->childs)
    {
        CheckEntities(g,0);
    }

    //UI::PopStyleColor(3);
}

void Solid::HierarchyTreeInterface::CheckEntities(GameObject* child, unsigned int it)
{


    ImVec2 pos = UI::GetCursorPos();
    pos.x += 20.f * (float)it;
    UI::SetCursorPos(pos);

    DrawEntity(child);


    for(GameObject* child : child->childs)
    {
        CheckEntities(child, it+1);
    }

}

void Solid::HierarchyTreeInterface::DrawEntity(GameObject* child)
{

    if(UI::Selectable((child->name + "##" +std::to_string(child->GetEntity())).c_str())
                        || (UI::IsAnyMouseDown() && UI::IsMouseHoveringRect(UI::GetItemRectMin(), UI::GetItemRectMax())))
        EditorInterface::selectedGO = child;

    if(UI::IsMouseHoveringRect(UI::GetItemRectMin(), UI::GetItemRectMax()) &&
            UI::IsMouseDragging(ImGuiMouseButton_Left) && child == EditorInterface::selectedGO)
        EditorInterface::draggingEnt = true;

    if(UI::IsMouseHoveringRect(UI::GetItemRectMin(), UI::GetItemRectMax()) &&
            EditorInterface::draggingEnt && !UI::IsAnyMouseDown())
    {
        EditorInterface::draggingEnt = false;
        Log::Send("Changed " + EditorInterface::selectedGO->name +
                             "'s parent to " + child->name, Log::ELogSeverity::DEBUG);
        /* broken
        child->parent->RemoveCurrent();
        EditorInterface::selectedGO->parent = child;
        child->AddToCurrent(EditorInterface::selectedGO->GetEntity());
        */
    }
}

