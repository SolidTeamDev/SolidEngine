//
// Created by ryan1 on 27/01/2021.
//

#ifndef SOLIDEDITOR_RESSOURCES_HPP
#define SOLIDEDITOR_RESSOURCES_HPP


#ifndef SOLID_API
#define SOLID_API __declspec(dllexport)
#endif

#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>
namespace fs = std::filesystem;
using uint = unsigned int ;

#include "ResourceType.hpp"
#include "ResourceMan.hpp"
#include "ResourceLoader.hpp"


#endif //SOLIDEDITOR_RESSOURCES_HPP
