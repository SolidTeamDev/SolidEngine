#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/Ressources.hpp"
#include "editor.hpp"
#include <chrono>
int main()
{
    try
    {
        Solid::Editor editor;
        editor.Run();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return EXIT_SUCCESS;
}
