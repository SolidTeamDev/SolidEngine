#pragma once

#ifndef SOLID_API
    #define SOLID_API //__declspec(dllexport)
#endif

#ifndef SOLID_EDITOR_API
#define SOLID_EDITOR_API //__declspec(dllexport)
#endif
using uint = unsigned int ;
#include "EngineGenerated/EntityMacros.h"