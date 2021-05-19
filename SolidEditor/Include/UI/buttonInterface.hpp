//
// Created by ryan1 on 17/05/2021.
//

#ifndef SOLIDLAUNCHER_BUTTONINTERFACE_HPP
#define SOLIDLAUNCHER_BUTTONINTERFACE_HPP


#include <string>
#include <unordered_map>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"

namespace Solid
{
	class ButtonInterface
	{
	public:
		//public members

	protected:
		//protected members
		std::unordered_map<std::string, ImageResource*> editorImage;
		std::unordered_map<std::string, std::shared_ptr<GL::Texture>> editorTex;
		bool compile = false;
		bool compileResult = false;
		bool popOpen = false;
	public:
		//public func
		ButtonInterface();
		~ButtonInterface() = default;



		void Draw();
	};
}




#endif //SOLIDLAUNCHER_BUTTONINTERFACE_HPP
