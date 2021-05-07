#pragma once

namespace Solid
{
    class FilesInterface
    {
	    bool        matNamePopup = false;
	    std::string matNamestr;
    public:
        FilesInterface()  = default;
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