#pragma once

#include "Build/solidAPI.hpp"

#include <string>

namespace Solid
{
    /**
     * @brief List of error codes
     */
    enum class ESolidErrorCode : int
    {
        S_NO_ERROR  = 0,
        S_ASSERT    = 1,     // Assertion error code
        S_INIT_ERROR= 2,
    };

    /**
     * @brief Class used for store error info to throw
     */
    class SOLID_API ThrowError
    {
    private:

        ESolidErrorCode errorCode;
        std::string errorDescription;

    public:

        ESolidErrorCode ErrorCode() const;
        std::string What() const;

        ThrowError(const std::string& _errorDescription, ESolidErrorCode _errorCode);
        ThrowError(const ThrowError& _copy) = delete;
        ThrowError(ThrowError&& _move) = delete;
    };
} //!namespace
