#include <string>
#include "UI/filesInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/engine.hpp"
#include "imgui_stdlib.h"
#include "GameCompiler/gameCompiler.hpp"
namespace Solid
{
    void FilesInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Files", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        std::vector<ResourcesPathData> data = Engine::GetInstance()->resourceManager.GetAllResourcesPath();
        counter -= Time::DeltaTime();
        if(counter <= 0.0)
        {
        	counter = 2.0;
        	root.childPaths.clear();
        	root.fileNames.clear();
	        for(auto& elt : data)
	        {
		        std::string type = "NONE";
		        switch (elt.RType)
		        {
			        case EResourceType::Mesh:
			        {
				        type="Mesh";
				        break;
			        }
			        case EResourceType::Shader:
			        {
				        type="Shader";
				        break;
			        }
			        case EResourceType::Material:
			        {
				        type="Material";
				        break;
			        }
			        case EResourceType::Compute:
			        {
				        type="Compute";
				        break;
			        }
			        case EResourceType::Image:
			        {
				        type="Image";
				        break;
			        }
			        case EResourceType::Anim:
			        {
				        type="Anim";
				        break;
			        }
			        case EResourceType::Audio:
			        {
				        type="Audio";
				        break;
			        }
			        case EResourceType::Skeleton:
			        {
				        type="Skeleton";
				        break;
			        }
			        case EResourceType::Scene:
			        {
				        type="Scene";
				        break;
			        }
			        case EResourceType::Prefab:
			        {
				        type="Prefab";
				        break;
			        }
			        default:
				        ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
				        break;
		        }
	        	if(elt.rPath.size() == 1)
		        {
			        root.fileNames.push_back(file{.fileNames=elt.RName, .ftype=type});
			        continue;
		        }
		        if(elt.rPath.empty())
			        continue;
		        filePathData* node = &root;
		        for (int i = 1; i < elt.rPath.size(); ++i)
		        {
			        if(node->childPaths.contains(elt.rPath[i]))
			        {
				        node = &(node->childPaths[elt.rPath[i]]);
				        continue;
			        }
			        else
			        {
				        node->childPaths.emplace(elt.rPath[i], filePathData{.folderName=elt.rPath[i], .parent=node});
				        node = &(node->childPaths[elt.rPath[i]]);
				        continue;

			        }
		        }
		        node->fileNames.push_back(file{.fileNames=elt.RName, .ftype=type});
	        }
        }
	    {
	    	if(UI::Button("Import Resource"))
		    {
	    		UI::OpenPopup("Importer");
		    }


	    	if(fileBrowser.showFileDialog("Importer", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,{0,0}, ".jpg,.png,.bmp,.obj,.fbx,.ogg,.wav,.jpeg,.simage,.smesh,.scompute,.svertfrag,.solidprefab,.saudio,.smaterial,.vert,.frag,.compute"))
		    {
	    		fs::path path = fileBrowser.selected_path;
			    ResourcesLoader loader;
			    loader.SetManager(&Engine::GetInstance()->resourceManager);
			    std::string fn = fileBrowser.selected_fn;
			    std::transform(fn.begin(), fn.end(), fn.begin(),
			                   [](unsigned char c){ return std::tolower(c); });
	    		if(fn.find("vert") != std::string::npos
	    		||fn.find("frag") != std::string::npos
	    		||fn.find("compute") != std::string::npos)
			    {
	    			path =path.parent_path();
	    			fs::path copy = ResourcesLoader::SolidPath;
	    			copy.append(path.filename().string());
	    			if(!fs::exists(copy))
	    				fs::create_directory(copy);
				    const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				    fs::copy(path, copy, opt);
				    ResourcePtrWrapper wrap{.r=nullptr};
				    loader.LoadRessourceNoAdd(copy, wrap);
				    if(!Engine::GetInstance()->resourceManager.IsResourceExist(wrap.r))
				    {
					    Engine::GetInstance()->resourceManager.AddResource(wrap.r);
				    }
				    else
				    {
				    	fs::remove(copy);
					    if(wrap.r != nullptr)
						    delete wrap.r;
				    }
			    }
	    		else
			    {
				    fs::copy(fileBrowser.selected_path, ResourcesLoader::SolidPath);
				    fs::path p = ResourcesLoader::SolidPath;
				    p.append(fileBrowser.selected_fn);
				    ResourcePtrWrapper wrap{.r=nullptr};
				    loader.LoadRessourceNoAdd(p, wrap);
				    if(!Engine::GetInstance()->resourceManager.IsResourceExist(wrap.r))
				    {
					    Engine::GetInstance()->resourceManager.AddResource(wrap.r);
				    }
				    else
				    {
					    fs::remove(p);
					    if(wrap.r != nullptr)
						    delete wrap.r;
				    }
			    }


		    }
	    	UI::Separator();

	        int imgSize = 32;
	    	if(currentFolder->parent != nullptr && UI::Button("..##previous"))
		    {
	    		currentFolder = currentFolder->parent;
		    }
		    for(auto& elt : currentFolder->childPaths)
		    {
		        if(UI::ImageButton((ImTextureID)editorTex["Folder"]->texId,ImVec2(imgSize,imgSize),ImVec2(0,1),ImVec2(1,0)))
		        {
		        	currentFolder = &elt.second;
		        }
                UI::SameLine();
                UI::SetCursorPosY(UI::GetCursorPosY()+imgSize/2);
                UI::Text(elt.first.c_str());
                UI::SetCursorPosY(UI::GetCursorPosY()-imgSize/2);

            }
		    for(auto& elt : currentFolder->fileNames)
		    {
		        std::string fileExt = elt.substr(elt.find('.') + 1);
                std::string img = "File";
		        if(fileExt == "png" || fileExt == "bmp" || fileExt == "jpg")
		            img = "ImgFile";
		        else if(fileExt == "wav")
		            img = "SoundFile";
                else if(fileExt == "SVertFrag")
                    img = "ShaderFile";

                UI::ImageButton((ImTextureID)editorTex[img]->texId,ImVec2(imgSize,imgSize),ImVec2(0,1),ImVec2(1,0));
                if(UI::BeginDragDropSource())
			    {
			    	UI::Text(elt.fileNames.c_str());
			    	UI::SetDragDropPayload(elt.ftype.c_str(), elt.fileNames.data(), elt.fileNames.size());


				    UI::EndDragDropSource();
			    }
		        UI::SameLine();
		        UI::SetCursorPosY(UI::GetCursorPosY()+imgSize/2);
		        UI::Text(elt.c_str());
                UI::SetCursorPosY(UI::GetCursorPosY()-imgSize/2);
            }
			    

		  
	    }

        DrawCreateFile();
	    DrawMatNamePopup();
	    UI::End();



    }

    void FilesInterface::DrawCreateFile()
    {

        if(UI::BeginPopupContextWindow("createFile"))
        {
            if(UI::BeginMenu("New"))
            {

	            if(UI::MenuItem("Create New Script"))
                {
	            	open = true;
                }

	            if (UI::MenuItem("Create Material"))
	            {

	            	matNamePopup = true;
	            	matNamestr = "";


	            }
                UI::EndMenu();
            }

            UI::EndPopup();
        }
        if(open)
        {
	        open = false;
	        fName = "";
        	UI::OpenPopup("FName");
        }

        if(UI::BeginPopup("FName"))
        {
	        UI::InputText("File Name",&fName);
	        if(UI::Button("Create"))
	        {
		        GameCompiler* Compiler = GameCompiler::GetInstance();
		        fs::path p = Compiler->srcPath.string() +"/Include/" + fName + ".hpp";
		        if(fName == "" || fs::exists(p))
		        {
					//error
		        }
		        else
		        	Compiler->CreateScript(fName);
	        }
        	UI::EndPopup();
        }
    }

	void FilesInterface::DrawMatNamePopup()
	{
    	if(matNamePopup)
	    {
    		UI::OpenPopup("MatNamePopUp");
	        matNamePopup = false;
	    }

		if (UI::BeginPopup("MatNamePopUp"))
		{
			UI::Text("Material Name :");
			UI::SameLine();
			UI::InputText("##MatName", &matNamestr);
			if(UI::Button("Create new Material"))
			{
				UI::ShowDemoWindow();
				Engine* engine = Engine::GetInstance();
				MaterialResource* mat =engine->resourceManager.CreateMaterial(matNamestr.c_str());
				mat->SetShader(engine->graphicsResourceMgr.GetShader("DefaultShader"));
				UI::CloseCurrentPopup();
			}
			UI::EndPopup();
		}

	}

	FilesInterface::FilesInterface()
	{
		root.folderName = "\\Assets\\";

        fs::path EditorAssets = fs::current_path();
        EditorAssets.append("EditorAssets");
        ResourcesLoader loader;
        {
            ResourcePtrWrapper wrap{.r=nullptr};
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/Folder.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("Folder", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("Folder", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/File.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("File", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("File", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/ImgFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("ImgFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("ImgFile", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/ShaderFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("ShaderFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("ShaderFile", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/SoundFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("SoundFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("SoundFile", Tex);
                }
            }
        }
	}
}
