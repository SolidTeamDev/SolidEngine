#include "Core/Debug/log.hpp"

namespace Solid
{
    class LogsInterface
    {
    public:
        LogsInterface()  = default;
        ~LogsInterface() = default;

        void Draw();

        bool p_open = true;
    };
}