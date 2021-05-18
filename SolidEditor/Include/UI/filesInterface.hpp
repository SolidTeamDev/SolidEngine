#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"
#include "ImGuiFileBrowser.h"

namespace Solid
{
    class FilesInterface
    {
	    bool        matNamePopup = false;
	    std::string matNamestr;
	    struct filePathData
	    {
	    	std::string folderName;
	    	filePathData* parent = nullptr;
	    	std::vector<std::string> fileNames;
	    	std::unordered_map<std::string, filePathData> childPaths;
	    } root;
	    double counter =0.0f;
	    filePathData* currentFolder = &root;

        std::unordered_map<std::string, ImageResource*> editorImage;
        std::unordered_map<std::string, std::shared_ptr<GL::Texture>> editorTex;
        
	    imgui_addons::ImGuiFileBrowser fileBrowser;
    public:
        FilesInterface();
        ~FilesInterface() = default;

        void Draw();
        void DrawCreateFile();
	    /**
		 * @brief Draws a poppu asking for a material name
		 */
	    void DrawMatNamePopup();

        bool p_open = true;
	    bool open = false;
	    std::string fName;
    };
}
