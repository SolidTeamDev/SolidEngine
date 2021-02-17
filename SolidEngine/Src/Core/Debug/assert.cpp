#include "Core/Debug/assert.hpp"

#include "Core/Debug/throwError.hpp"

namespace Solid
{
    void s_assert(bool expr, int line, const std::string& file)
    {
        if(expr)
            return;

        std::string str = "Assertion at line " + std::to_string(line) + " in file "+file;
        throw ThrowError(str, ESolidErrorCode::S_ASSERT);
    }
} //!namespace
