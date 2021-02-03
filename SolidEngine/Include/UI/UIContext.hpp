#pragma once

#include "Build/solidAPI.hpp"

namespace Solid
{
    /**
     * @brief Base UI context class.
     */
    class SOLID_API UIContext
    {
    public:
        UIContext();
        ~UIContext();

        void Initialize();
        void Release();
    };
} //!namespace