#pragma once

#include "Build/SolidAPI.hpp"

#include <string>

namespace Solid
{
    /**
     * @brief List of error codes
     */
    enum ESolidErrorCode : int
    {
        S_NO_ERROR  = 0,
        S_ASSERT    = 1     // Assertion error code
    };

    /**
     * @brief Class used for store error info to throw
     */
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

    /**
     * @brief Exit program if expression is false
     * @param expr Expression to test
     * @param line The line of the called function
     * @param file The file of the called function
     */
    void SOLID_API assert(bool expr, int line, const std::string& file);
    #define sAssert(expr) assert(expr,__LINE__,__FILE__)

    /**
     * @brief Class used for store every log during the program lifetime
     */
    class Log
    {

    public:

    };
} //!namespace


