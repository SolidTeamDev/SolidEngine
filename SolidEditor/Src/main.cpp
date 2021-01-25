#include <iostream>

#include "editor.hpp"

int main()
{

    Solid::test();

    try
    {
        Solid::Editor editor;
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}