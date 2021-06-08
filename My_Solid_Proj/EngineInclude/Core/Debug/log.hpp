#pragma once

#include "Build/solidAPI.hpp"

#include <string>
#include <array>
#include <fstream>

#undef ERROR

constexpr int maxLogMessage = 100;

namespace Solid {
    /**
     * @brief Class used for store every log during the program lifetime
     */
    class SOLID_API Log {
    public:

        enum class ELogSeverity {
            INFO,       // Display info information
            DEBUG,      // Used to debug something
            WARNING,    // Warning message
            ERROR,      // Used when an error occurred
            CRITICAL    // Used when an critical error occurred (like close program)
        };

    private:

        struct LogMessage {
            ELogSeverity logSeverity;
            std::string logMessage;
        };
        static std::array<LogMessage, maxLogMessage> logMessageList;
        static int logIndex;
        static int logCount;
        static std::ofstream logFile;
    public:

        Log();
        ~Log();

	    static void Send(const std::string &_logMessage, const ELogSeverity &_severity = ELogSeverity::INFO);
        static LogMessage GetLogAt(unsigned int idx);
        static void Clear();
    };
}