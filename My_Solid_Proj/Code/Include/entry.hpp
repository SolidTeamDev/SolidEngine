//
// Created by ryan1 on 22/01/2021.
//

#ifndef DLL_LOADER_GAMETEST_H
#define DLL_LOADER_GAMETEST_H

#ifdef __cplusplus

#include <string>
#include "Refureku/Refureku.h"

extern "C" int __declspec(dllexport) __stdcall Entry(); 
extern "C" std::size_t __declspec(dllexport) __stdcall GetClass(const std::string& );
extern "C" std::size_t __declspec(dllexport) __stdcall GetNamespace(const std::string& );

#endif




#endif //DLL_LOADER_GAMETEST_H