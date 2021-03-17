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
    Engine *engine = Engine::GetInstance();
    for (GameObject *g : engine->ecsManager.GetWorld()->childs)
    {
        CheckEntities(g, 0);
    }

    if (EditorInterface::draggingEnt && !UI::IsAnyMouseDown() && !UI::IsAnyItemHovered() && UI::IsWindowHovered())
    {
        EditorInterface::draggingEnt = false;
        if (EditorInterface::selectedGO->parent == nullptr)
            return;

        EditorInterface::selectedGO->ReParentCurrent(engine->ecsManager.GetWorld());
    }
}

void Solid::HierarchyTreeInterface::CheckEntities(GameObject* child, unsigned int it)
{
    ImVec2 pos = UI::GetCursorPos();
    pos.x += 20.f * (float)it;
    UI::SetCursorPos(pos);

    if (DrawEntity(child))
        UI::TreePop();

    for(GameObject* child : child->childs)
    {
        CheckEntities(child, it+1);

    }
}

bool Solid::HierarchyTreeInterface::DrawEntity(GameObject* child)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

    if (child->childs.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;
    bool result = UI::TreeNodeEx((child->name + "##" +std::to_string(child->GetEntity())).c_str(), flags);
    if (UI::IsAnyMouseDown() && UI::IsItemHovered())
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

		if(child != EditorInterface::selectedGO)
            EditorInterface::selectedGO->ReParentCurrent(child);

    }
    return result;
}

