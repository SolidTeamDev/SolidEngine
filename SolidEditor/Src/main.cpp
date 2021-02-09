#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "Ressources.hpp"
#include <chrono>
struct v3
{
    float x;
    float y;
    float z;
};
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

    std::system("Pause");
    return EXIT_SUCCESS;
}
