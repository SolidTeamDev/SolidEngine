#include <iostream>
#include "Core/engine.hpp"
#include "Refureku/Refureku.h"
#include "entry.hpp"
int Entry()
{
     std::cout << "Hello World " << std::endl;
     return 0;
}
std::size_t  GetClass(const std::string& _s)
{
     return (std::size_t) rfk::Database::getClass(_s);
}
std::size_t  GetNamespace(const std::string& _s)
{
     return (std::size_t) rfk::Database::getNamespace(_s);
}












