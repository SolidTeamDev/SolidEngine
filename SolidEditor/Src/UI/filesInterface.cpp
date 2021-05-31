#include <string>
#include "UI/filesInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/engine.hpp"
#include "imgui_stdlib.h"
#include "GameCompiler/gameCompiler.hpp"
#include "UI/winUI.hpp"
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
			std::string currentfolderName = currentFolder->folderName;

        	root.childPaths.clear();
        	root.fileNames.clear();
        	std::function<void(fs::path, filePathData*)> init = [&](fs::path _path, filePathData* node)
        	{
				if (!fs::exists(_path))
					return;
        		for(auto& item : fs::directory_iterator(_path))
        		{
        			if(fs::is_directory(item))
			        {
				        std::string name = item.path().filename().string();
				        std::transform(name.begin(), name.end(), name.begin(),
				                       [](unsigned char c){ return std::tolower(c); });
                        bool isCubemap = false;
				        for(auto& item2 : fs::directory_iterator(item))
                        {
                            if(!item2.is_directory())
                            {
                                std::string ext = item2.path().filename().string();
                                std::transform(ext.begin(), ext.end(), ext.begin(),
                                               [](unsigned char c){ return std::tolower(c); });
                                if(ext.find(".cubemap") != std::string::npos)
                                {
                                    isCubemap = true;
                                }
                            }
                        }
				        bool isShader = (name.find("shader") != std::string::npos || name.find("compute") != std::string::npos);
				        if(isShader || isCubemap)
					        continue;

				        else
				        {
					        name = item.path().filename().string();
					        filePathData* child = &node->childPaths.emplace(name, filePathData{.folderName=name, .parent=node}).first->second;
					        init(item.path(), child);
				        }
			        }
        		}
        	};
        	init(ResourcesLoader::SolidPath, &root);
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
                    case EResourceType::Cubemap:
                    {
                        type="Cubemap";
                        break;
                    }
			        default:
				        ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
				        break;
		        }
	        	if(elt.rPath.size() == 1)
		        {
			        root.fileNames.push_back(file{.fileNames=elt.RName, .ftype=type, .RPtr=elt.RPtr});
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
		        node->fileNames.push_back(file{.fileNames=elt.RName, .ftype=type, .RPtr=elt.RPtr});
	        }
	        std::function<bool(filePathData*)> f = [&](filePathData* node)->bool
	        {
		        if(node->folderName == currentfolderName)
		        {
			        currentFolder = node;
		        	return true;
		        }
		        if(node->childPaths.contains(currentfolderName))
		        {
		        	currentFolder = &node->childPaths[currentfolderName];
		        	return true;
		        }
		        for(auto& elt : node->childPaths)
		        {
		        	return f(&elt.second);

		        }
		        return false;
	        };
	        bool b =f(&root);
	        if(!b)
	        	currentFolder = &root;
        }
	    {
	    	if(UI::Button("Import Resource"))
		    {
	    		//UI::OpenPopup("Importer");
	    		const char* filter = "All Valid Types\0*.OBJ;*.FBX;*.SMESH;*.SANIM;*.PNG;*.BMP;*.SIMAGE;*.JPG;*.JPEG;*.VERT;*.SVERTFRAG;*.COMPUTE;*.SCOMPUTE;*.SMATERIAL;*.SOLIDSCENE;*.WAV;*.OGG;*.SAUDIO;*.SOLIDPREFAB\0Meshes\0*.OBJ;*.FBX;*.SMESH\0Anims\0*.FBX;*.SANIM\0Images\0*.PNG;*.BMP;*.SIMAGE;*.JPG;*.JPEG\0Shader\0*.VERT;*.SVERTFRAG\0Compute Shader\0*.COMPUTE;*.SCOMPUTE\0Material\0*.SMATERIAL\0Scene\0*.SOLIDSCENE\0Audio\0*.WAV;*.OGG;*.SAUDIO\0Prefab\0*.SOLIDPREFAB\0";
			    HasChosen hc =WinOpenFileMultiSelect(filter);
			    if(hc.b)
			    {
				    std::vector<fs::path> sorted;
				    std::vector<fs::path> sortedPass2;
			    	for(const fs::path& elt : hc.vstr)
			    	{
			    	    for(const fs::path& elt2 : hc.vstr)
			    	    {
			    	        if(elt == elt2)
					            continue;
			    	        if(elt.filename().string().find(elt2.filename().string()) != std::string::npos)
				            {
			    	        	sorted.push_back(elt2);
				            }
			    	    }
			    	}
			    	for(const fs::path& elt : hc.vstr)
			    	{
			    		bool alreadySorted = false;
			    	    for(fs::path& elt2 : sorted)
			    	    {
			    	        if(elt.filename().string().find(elt2.filename().string()) != std::string::npos)
				            {
					            alreadySorted = true;
				            }
			    	    }
			    	    if(!alreadySorted)
			    	    	sorted.push_back(elt);
			    	}

			    	for(fs::path& elt :sorted)
			    	{
			    		fs::path path = elt;
					    ResourcesLoader loader;
					    loader.SetManager(&Engine::GetInstance()->resourceManager);
					    std::string fn = elt.filename().string();
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
						    fs::copy(elt, ResourcesLoader::SolidPath);
						    fs::path p = ResourcesLoader::SolidPath;
						    p.append(elt.filename().string());
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
			    }



		    }


	    	UI::Separator();

	        int imgSize = 32;

	    	if(currentFolder->parent != nullptr )
		    {
	    		UI::PushID("previousF");
	    		if(UI::ImageButton((ImTextureID)editorTex["Folder"]->texId,ImVec2(imgSize,imgSize),ImVec2(0,1),ImVec2(1,0)))
			    {
				    currentFolder = currentFolder->parent;
			    }
	    		UI::PopID();
			    if(UI::BeginDragDropTarget())
			    {

				    const ImGuiPayload* drop=UI::AcceptDragDropPayload(nullptr);

				    if(drop != nullptr )
				    {
				    	filePathData* newFolder = currentFolder->parent;

					    Resource* r = *((Resource**)drop->Data);
					    fs::path oldp = ResourcesLoader::SolidPath;
					    for (int i = 1; i < r->path.size(); ++i)
					    {
						    oldp.append(r->path[i]);
					    }
					    r->path.clear();
					    for (  ; newFolder != nullptr ; )
					    {
					    	r->path.push_front(newFolder->folderName);

						    newFolder = newFolder->parent;
					    }
					    fs::path newp = ResourcesLoader::SolidPath;
					    for (int i = 1; i < r->path.size(); ++i)
					    {
						    newp.append(r->path[i]);
					    }
					    fs::path pArray[2];
					    fs::path newpArray[2]{newp,newp};
					    int i = 0;
					    for(auto& elt : fs::directory_iterator(oldp))
					    {
					        if(elt.path().filename().string().find(r->name) != std::string::npos)
					        {

						        if(fs::is_directory(elt))
						        {
							        fs::path create = newp;
							        create.append(elt.path().filename().string());
							        fs::create_directory(create);
							        fs::copy(elt.path(), create);
						        }
						        else
						        {
							        fs::copy(elt.path(), newp);
						        }
						        pArray[i] = elt.path();
					        	++i;
					        }
					    }
					    for (int j = 0; j < i; ++j)
					    {
					    	if(fs::is_directory(pArray[j]))
					    		fs::remove_all(pArray[j]);
					    	else
					    	    fs::remove(pArray[j]);
					    }
					    

					    counter = -1;
				    }
				    UI::EndDragDropTarget();
			    }
			    UI::SameLine();
			    UI::SetCursorPosY(UI::GetCursorPosY()+imgSize/2);
			    UI::Text("..");
			    UI::SetCursorPosY(UI::GetCursorPosY()-imgSize/2);

		    }
		    for(auto& elt : currentFolder->childPaths)
		    {
		    	UI::PushID(elt.first.c_str());
		        if(UI::ImageButton((ImTextureID)editorTex["Folder"]->texId,ImVec2(imgSize,imgSize),ImVec2(0,1),ImVec2(1,0)))
		        {
		        	currentFolder = &(currentFolder->childPaths[elt.first]);

		        }
			    UI::PopID();
			    if(UI::BeginDragDropTarget())
			    {

				    const ImGuiPayload* drop=UI::AcceptDragDropPayload(nullptr);

				    if(drop != nullptr )
				    {
					    filePathData* newFolder = &currentFolder->childPaths[elt.first];

					    Resource* r = *((Resource**)drop->Data);
					    fs::path oldp = ResourcesLoader::SolidPath;
					    for (int i = 1; i < r->path.size(); ++i)
					    {
						    oldp.append(r->path[i]);
					    }
					    r->path.clear();
					    for (  ; newFolder != nullptr ; )
					    {
						    r->path.push_front(newFolder->folderName);

						    newFolder = newFolder->parent;
					    }
					    fs::path newp = ResourcesLoader::SolidPath;
					    for (int i = 1; i < r->path.size(); ++i)
					    {
						    newp.append(r->path[i]);
					    }
					    fs::path pArray[2];
					    fs::path newpArray[2]{newp,newp};
					    int i = 0;
					    for(auto& eltpath : fs::directory_iterator(oldp))
					    {
						    if(eltpath.path().filename().string().find(r->name) != std::string::npos)
						    {
								if(fs::is_directory(eltpath))
								{
									fs::path create = newp;
									create.append(eltpath.path().filename().string());
									fs::create_directory(create);
									fs::copy(eltpath.path(), create);
								}
								else
								{
									fs::copy(eltpath.path(), newp);
								}

							    pArray[i] = eltpath.path();
							    ++i;
						    }
					    }
					    for (int j = 0; j < i; ++j)
					    {
					    	if(!fs::is_directory(pArray[j]))
						        fs::remove(pArray[j]);
					    	else
					    		fs::remove_all(pArray[j]);
					    }
					    ResourcesLoader loader;
					    //loader.ReLoadRessource(r);
					    counter = -1;
				    }
				    UI::EndDragDropTarget();
			    }
                UI::SameLine();
                UI::SetCursorPosY(UI::GetCursorPosY()+imgSize/2);
                UI::Text(elt.first.c_str());
                UI::SetCursorPosY(UI::GetCursorPosY()-imgSize/2);

            }
		    for(auto& elt : currentFolder->fileNames)
		    {

                std::string img = "File";
		        if(elt.ftype == "Image" )
		            img = "ImgFile";
		        else if(elt.ftype == "Audio" )
		            img = "SoundFile";
                else if(elt.ftype == "Shader" )
                    img = "ShaderFile";
                else if(elt.ftype == "Mesh")
                    img = "ObjFile";
                else if(elt.ftype == "Prefab")
                    img = "PrefabFile";
                else if(elt.ftype == "Material")
                    img = "MatFile";
			    UI::PushID(elt.fileNames.c_str());
                bool press =UI::ImageButton((ImTextureID)editorTex[img]->texId,ImVec2(imgSize,imgSize),ImVec2(0,1),ImVec2(1,0));

                if(press && (elt.ftype == "Cubemap"))
                {
                    Engine::GetInstance()->renderer->_map = Engine::GetInstance()->graphicsResourceMgr.GetCubemap("Skybox");
                }

                UI::PopID();
                if(UI::BeginDragDropSource())
			    {
			    	UI::Text(elt.fileNames.c_str());
			    	UI::SetDragDropPayload(elt.ftype.c_str(), &(elt.RPtr), sizeof(Resource**));


				    UI::EndDragDropSource();
			    }
		        UI::SameLine();
		        UI::SetCursorPosY(UI::GetCursorPosY()+imgSize/2);
		        UI::Text(elt.fileNames.c_str());
                UI::SetCursorPosY(UI::GetCursorPosY()-imgSize/2);
            }
			    

		  
	    }

        DrawCreateFile();
	    DrawMatNamePopup();
	    DrawFolderPopup();
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
	            if (UI::MenuItem("Create Folder"))
	            {

		            folderPopup = true;
		            folderstr = "";


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
		        {
		        	Compiler->CreateScript(fName);
		            UI::CloseCurrentPopup();
		        }
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
	void FilesInterface::DrawFolderPopup()
	{
		if(folderPopup)
		{
			UI::OpenPopup("FolderPopUp");
			folderstr = "";
			folderPopup = false;
		}

		if (UI::BeginPopup("FolderPopUp"))
		{
			UI::Text("Folder Name :");
			UI::SameLine();
			UI::InputText("##Folder", &folderstr);
			if(UI::Button("Create new Folder"))
			{
				fs::path p = ResourcesLoader::SolidPath;
				std::deque<std::string> pathList;
				if(currentFolder != &root)
				{
					filePathData* node = currentFolder;
					while(node->parent != nullptr)
					{
						pathList.push_front(node->folderName);
						node = node->parent;
					}
					for(auto& elt : pathList)
					{
						p.append(elt);
					}
				}
				p.append(folderstr);
				fs::create_directory(p);
				currentFolder->childPaths.emplace(folderstr, filePathData{.folderName=folderstr, .parent=currentFolder});
				UI::CloseCurrentPopup();
			}
			UI::EndPopup();
		}

	}

	FilesInterface::FilesInterface()
	{
		root.folderName = "Assets\\";
		currentFolder =&root;
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
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/ObjFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("ObjFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("ObjFile", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/MatFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("MatFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("MatFile", Tex);
                }
            }
        }
        {
            ResourcePtrWrapper wrap;
            loader.LoadRessourceNoAdd( EditorAssets.string() + "/PrefabFile.png", wrap);
            if(wrap.r != nullptr && wrap.r->GetType() == EResourceType::Image)
            {
                editorImage.emplace("PrefabFile", (ImageResource*)wrap.r);
                std::shared_ptr<GL::Texture> Tex = std::make_shared<GL::Texture>((ImageResource*)wrap.r);
                if(Tex != nullptr)
                {
                    editorTex.emplace("PrefabFile", Tex);
                }
            }
        }
	}

	FilesInterface::~FilesInterface()
	{
	}
}
