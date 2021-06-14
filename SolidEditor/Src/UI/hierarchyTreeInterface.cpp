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

        UI::BeginChild("##HierarchyChild", UI::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);




        if (UI::IsWindowHovered() && UI::IsAnyMouseDown())
            EditorInterface::selectedGO = nullptr;
		UI::Separator();
        DrawEntities();

        DrawCreateObject();
		UI::EndChild();
	    if(UI::BeginDragDropTarget())
	    {

		    const ImGuiPayload* drop=UI::AcceptDragDropPayload("Prefab");
		    if(drop != nullptr)
		    {
			    Resource* r = *((Resource**)drop->Data);
			    Engine::GetInstance()->ecsManager.Instantiate(r->name, nullptr);

		    }
		    else
		    {
			    const ImGuiPayload* drop=UI::AcceptDragDropPayload("ReChildGO");
			    if(drop != nullptr)
			    {
				    GameObject* go = *((GameObject**)drop->Data);
				    go->ReParentCurrent(Engine::GetInstance()->ecsManager.GetWorld());
			    }
			    else
			    {
				    drop=UI::AcceptDragDropPayload("Cubemap");
				    if(drop != nullptr)
				    {
					    Resource* r = *((Resource**)drop->Data);
					    Engine::GetInstance()->renderer->_map = Engine::GetInstance()->graphicsResourceMgr.GetCubemap(r->name.c_str());
				    }
				    else
				    {
					    drop=UI::AcceptDragDropPayload("Scene");
					    if(drop != nullptr)
					    {
						    Resource* r = *((Resource**)drop->Data);
						    Engine::GetInstance()->LoadScene(r->name.c_str());
						    EditorInterface::selectedGO = nullptr;
					    }
				    }
			    }
		    }
		    UI::EndDragDropTarget();
	    }
        UI::End();
    }
}

void Solid::HierarchyTreeInterface::DrawCreateObject()
{
    Engine* engine = Engine::GetInstance();
	bool create = true;
	if(create)
	{
		create = false;
	}
    if(UI::BeginPopupContextWindow("createObject"))
    {
        if(UI::BeginMenu("New"))
        {
            if(UI::MenuItem("GameObject"))
            {
                GameObject* tmp = nullptr;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                EditorInterface::selectedGO = tmp;

            }
            UI::Separator();
            if(UI::MenuItem("Cube"))
            {
                GameObject* tmp = nullptr;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                engine->ecsManager.AddComponent(tmp,MeshRenderer(
                		engine->graphicsResourceMgr.GetMesh("cube.obj"))
                );
                EditorInterface::selectedGO = tmp;

            }
            if(UI::MenuItem("Sphere"))
            {
                GameObject* tmp = nullptr;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                engine->ecsManager.AddComponent(tmp,MeshRenderer(
                        engine->graphicsResourceMgr.GetMesh("sphere.obj"))
                );
                EditorInterface::selectedGO = tmp;
            }
            if(UI::MenuItem("Solid"))
            {
                GameObject* tmp = nullptr;

                if (EditorInterface::selectedGO != nullptr)
                    tmp = engine->ecsManager.CreateEntity(EditorInterface::selectedGO->GetEntity());
                else
                    tmp = engine->ecsManager.CreateEntity();

                engine->ecsManager.AddComponent(tmp,MeshRenderer(
		                engine->graphicsResourceMgr.GetMesh("solid.obj"))
                );
                EditorInterface::selectedGO = tmp;
            }
            UI::EndMenu();
        }
	    if(EditorInterface::selectedGO != nullptr)
	    {
            UI::Separator();
		    if(UI::MenuItem("Remove GameObject"))
		    {
			    if (EditorInterface::selectedGO != nullptr)
			    {
			    	engine->ecsManager.DestroyEntity(EditorInterface::selectedGO->GetEntity());
				    EditorInterface::selectedGO = nullptr;
			    }

		    }
	    }

		    if(UI::MenuItem("Create / Update Prefab"))
		    {
				if(EditorInterface::selectedGO != nullptr)
				{
					PrefabResource* prefab = engine->resourceManager.GetPrefabByName(EditorInterface::selectedGO->name.c_str());
					if(prefab != nullptr)
					{
						prefab->UpdatePrefab(EditorInterface::selectedGO);
					}
					else
					{
						engine->resourceManager.CreatePrefab(EditorInterface::selectedGO);
					}
				}
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

    //modif dnd
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
    if(UI::BeginDragDropTarget())
    {
	    const ImGuiPayload* drop=UI::AcceptDragDropPayload("ReChildGO");
	    if(drop != nullptr)
	    {
		    GameObject* go = *((GameObject**)drop->Data);
		    go->ReParentCurrent(child);
	    }
	    UI::EndDragDropTarget();
    }
    if(UI::BeginDragDropSource())
	{
		UI::Text(child->name.c_str());
		UI::SetDragDropPayload("ReChildGO", &(child), sizeof(GameObject**));


		UI::EndDragDropSource();
	}
    if (UI::IsAnyMouseDown() && UI::IsItemHovered())
        EditorInterface::selectedGO = child;

    //modif dnd
    return result;
}

