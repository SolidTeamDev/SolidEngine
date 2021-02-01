#include <iostream>

#include "editor.hpp"

int main()
{
    try
    {
        Solid::Editor editor;
        editor.Loop();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}