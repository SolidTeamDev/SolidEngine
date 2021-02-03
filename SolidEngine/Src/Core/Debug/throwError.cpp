#include "Core/Debug/throwError.hpp"

namespace Solid
{
    ESolidErrorCode ThrowError::ErrorCode() const
    {
        return errorCode;
    }

    std::string ThrowError::What() const
    {
        return errorDescription;
    }

    ThrowError::ThrowError(const std::string &_errorDescription, ESolidErrorCode _errorCode)
    {
        errorDescription    = _errorDescription;
        errorCode           = _errorCode;
    }
} //!namespace
