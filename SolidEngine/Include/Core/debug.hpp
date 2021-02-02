#pragma once

#include "Build/SolidAPI.hpp"

#include <string>

namespace Solid
{
    enum ESolidErrorCode : int
    {
        S_NO_ERROR  = 0,
        S_ASSERT    = 1
    };

    class SOLID_API ThrowError
    {
        int errorCode;
        std::string errorDescription;
    public:
        int ErrorCode() const;
        std::string What() const;

        ThrowError(const std::string& _errorDescription, int _errorCode);
        ThrowError(const ThrowError& _copy) = delete;
        ThrowError(ThrowError&& _move) = delete;
    };

    void SOLID_API assert(bool expr, int line, const std::string& file);
    #define sAssert(expr) assert(expr,__LINE__,__FILE__)

    class Log
    {

    };
} //!namespace


