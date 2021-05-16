//
// Created by ryan1 on 16/05/2021.
//

#ifndef SOLIDLAUNCHER_SYSTEMCMD_HPP
#define SOLIDLAUNCHER_SYSTEMCMD_HPP
#ifdef _WIN32

#define SOLID_WIN 1
#include <windows.h>

int SOLID_API wincmd(std::string cmd, std::string& output, unsigned int maxOutput);


#endif
#endif //SOLIDLAUNCHER_SYSTEMCMD_HPP
