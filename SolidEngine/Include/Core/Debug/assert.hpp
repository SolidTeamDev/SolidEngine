#pragma once

#include "Build/solidAPI.hpp"

#include <string>

namespace Solid
{
    /**
     * @brief Exit program if expression is false
     * @param expr Expression to test
     * @param line The line of the called function
     * @param file The file of the called function
     */
    void SOLID_API assert(bool expr, int line, const std::string& file);
    #define sAssert(expr) assert(expr,__LINE__,__FILE__)
} //!namespace
