#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"
#include "ImGuiFileBrowser.h"

namespace Solid
{
	struct file
	{
		std::string fileNames;
		std::string ftype;
		Resource* RPtr = nullptr;
	};
	struct filePathData
	{


		std::string folderName;
		filePathData* parent = nullptr;
		std::vector<file> fileNames;
		std::unordered_map<std::string, filePathData> childPaths;
	};
    class FilesInterface
    {
	    bool        matNamePopup = false;
	    std::string matNamestr;
	    bool        folderPopup = false;
	    std::string folderstr;

	    struct filePathData root;

	    double counter =0.0f;
        std::unordered_map<std::string, ImageResource*> editorImage;

	    std::unordered_map<std::string, std::shared_ptr<GL::Texture>> editorTex;
	    imgui_addons::ImGuiFileBrowser fileBrowser;

	    bool test = false;
    public:

	    filePathData* currentFolder ;
	    FilesInterface();
        ~FilesInterface() ;

        void Draw();
        void DrawCreateFile();
	    /**
		 * @brief Draws a poppu asking for a material name
		 */
	    void DrawMatNamePopup();
	    void DrawFolderPopup();

        bool p_open = true;
	    bool open = false;
	    std::string fName;
    };
}
