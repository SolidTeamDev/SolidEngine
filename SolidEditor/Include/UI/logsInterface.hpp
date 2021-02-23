#include "Core/Debug/log.hpp"

namespace Solid
{
    class LogsInterface
    {
    private:
        bool p_open = true;
    public:
        LogsInterface()  = default;
        ~LogsInterface() = default;

        void Draw();
    };
}