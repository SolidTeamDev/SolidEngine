#include <iostream>

#include "editor.hpp"

#include "Core/Debug/log.hpp"

int main()
{
    try
    {
        Solid::Editor editor;
        Solid::Log::Send("Test",Solid::Log::ELogSeverity::DEBUG);
        editor.Loop();
    }
    catch(const Solid::ThrowError& e)
    {
        Solid::Log::Send("Solid Engine exit with error code (" + std::to_string((int)e.ErrorCode()) + ") : \n" + e.What(),Solid::Log::ELogSeverity::CRITICAL);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}