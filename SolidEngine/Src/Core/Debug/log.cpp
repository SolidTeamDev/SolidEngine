#include "Core/Debug/log.hpp"

#include <iostream>

namespace Solid
{
    int Log::logIndex = 0;
    int Log::logCount = 0;
    std::array<Log::LogMessage,maxLogMessage> Log::logMessageList;
    std::ofstream Log::logFile;

    Log::Log()
    {
        logFile.open("Log.log");
    }

    Log::~Log()
    {
        logFile.close();
    }

    void Log::Send(const std::string &_logMessage, const Log::ELogSeverity &_severity)
    {
        if(logCount == maxLogMessage)
        {
            for (size_t i = 1; i < maxLogMessage ; i++)
            {
                logMessageList.at(i-1) = logMessageList.at(i);
            }
        }
        else
            logCount++;

        LogMessage logMessage
        {
            _severity,
            _logMessage
        };

        logMessageList.at(logIndex) = logMessage;

        std::string severityName;

        switch (_severity)
        {
            case ELogSeverity::INFO:
                severityName = "[INFO]";
                break;
            case ELogSeverity::DEBUG:
                severityName = "[DEBUG]";
                break;
            case ELogSeverity::WARNING:
                severityName = "[WARNING]";
                break;
            case ELogSeverity::ERROR:
                severityName = "[ERROR]";
                break;
            case ELogSeverity::CRITICAL:
                severityName = "[CRITICAL]";
                break;
        }

        logFile << severityName << ": " << _logMessage << std::endl;

        #ifndef NDEBUG
            std::cout << severityName << ": " << _logMessage << std::endl;
        #endif

        logIndex = logIndex == maxLogMessage - 1 ? logIndex : logIndex+1;
    }

    Log::LogMessage Log::GetLogAt(unsigned int idx)
    {
        return logMessageList.at(idx);
    }
}
