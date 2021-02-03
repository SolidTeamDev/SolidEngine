#include <iostream>

#include "editor.hpp"

int main()
{
    try
    {
        Solid::Editor editor;
        editor.Loop();
    }
    catch(const Solid::ThrowError& e)
    {
        std::cerr << "Solid Engine exit with error code (" << (int)e.ErrorCode() << ") : \n" << e.What() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}