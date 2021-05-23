//
// Created by ryan1 on 21/04/2021.
//

#ifndef SOLIDLAUNCHER_LAUNCHER_HPP
#define SOLIDLAUNCHER_LAUNCHER_HPP
#include "UI/solidUI.hpp"
#include "ImGuiFileBrowser.h"
namespace Solid
{

	class Launcher
	{
	private:
		class Engine_Cleaner : public EngineCleanerInterface
		{
		public:
			Engine_Cleaner()
			{

			}

			~Engine_Cleaner()
			{

			}
		}Clean;
		std::string ProjName;
		std::string ErrorStr;
		std::string ProjPath;
		fs::path root;

	public:
		bool needClose = false;
		imgui_addons::ImGuiFileBrowser fileBrowser;
		Engine* engine = nullptr;
		Launcher();
		~Launcher();
		void Run();
		void UpdateInterface();

	};
}

#endif //SOLIDLAUNCHER_LAUNCHER_HPP
