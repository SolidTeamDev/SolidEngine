#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/ressources.hpp"
#include "editor.hpp"
#include <chrono>
int main()
{
    try
    {
        Solid::Editor editor;
        editor.Run();
    }
    catch(const Solid::ThrowError& e)
    {
        std::cerr << "Error(" << (int)e.ErrorCode() << ") : " << e.What() << std::endl;
    }
    
    return EXIT_SUCCESS;
}
