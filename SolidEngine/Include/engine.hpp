#pragma once

#ifndef SOLID_API
    #define SOLID_API __declspec(dllexport)
#endif


#include "Ressources.hpp"


namespace Solid
{

    class Engine
    {
    public:
        Engine() = default;
        virtual ~Engine() = default;
    };

    void SOLID_API test();

} //!namespace
