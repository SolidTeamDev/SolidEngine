#pragma once

#include "Build/solidAPI.hpp"

#include <string>
#include <array>

constexpr int maxLogMessage = 100;

namespace Solid
{
    /**
     * @brief Class used for store every log during the program lifetime
     */
    class SOLID_API Log
    {
    public:

        enum class ELogSeverity
        {
            INFO,       // Display info information
            DEBUG,      // Used to debug something
            WARNING,    // Warning message
            ERROR,      // Used when an error occurred
            CRITICAL    // Used when an critical error occurred (like close program)
        };

    private:



        static int logIndex;
    public:
        struct LogMessage
        {
            ELogSeverity logSeverity;
            std::string  errorMessage;
        };
        static std::array<LogMessage,maxLogMessage> logMessageList;

        Log();

        static void Send(const std::string& _errorMessage, const ELogSeverity& _severity = ELogSeverity::INFO);
    };
} //!namespace
