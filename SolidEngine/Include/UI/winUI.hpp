//
// Created by ryan1 on 21/05/2021.
//

#ifdef _WIN32
#ifndef SOLIDLAUNCHER_WINUI_HPP


#define SOLIDLAUNCHER_WINUI_HPP
#include "Build/solidAPI.hpp"
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

#include <windows.h>
#include <shlobj_core.h>


namespace Solid
{



	struct SOLID_API HasChosen
	{
		bool b = false;
		std::vector<std::string> vstr;
	};


	HasChosen SOLID_API WinSelectFolder();

	HasChosen SOLID_API WinOpenFile(const char* filter);

	HasChosen SOLID_API WinSaveFile(const char* filter);
	HasChosen SOLID_API WinOpenFileMultiSelect(const char* filter);

#endif
}
#endif //SOLIDLAUNCHER_WINUI_HPP
