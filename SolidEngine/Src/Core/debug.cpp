#include "Core/debug.hpp"

#include <stdexcept>

namespace Solid
{
    int ThrowError::ErrorCode() const
    {
        return errorCode;
    }

    std::string ThrowError::What() const
    {
        return errorDescription;
    }

    ThrowError::ThrowError(const std::string &_errorDescription, int _errorCode)
    {
        errorDescription    = _errorDescription;
        errorCode           = _errorCode;
    }

    void assert(bool expr, int line, const std::string& file)
    {
        if(expr)
            return;

        std::string str = "Assertion at line " + std::to_string(line) + " in file "+file;
        throw ThrowError(str, S_ASSERT);
    }
} //!namespace