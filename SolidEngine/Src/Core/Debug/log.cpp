#include "Core/Debug/log.hpp"

namespace Solid
{
    int Log::logIndex = 0;
    std::array<Log::LogMessage,maxLogMessage> Log::logMessageList;

    Log::Log()
    {

    }

    void Log::Send(const std::string &_errorMessage, const Log::ELogSeverity &_severity)
    {
        if(logMessageList.size() == maxLogMessage)
        {
            for (size_t i = 1; i < maxLogMessage ; i++)
            {
                logMessageList.at(i-1) = logMessageList.at(i);
            }
        }

        LogMessage logMessage
        {
            _severity,
            _errorMessage
        };

        logMessageList.at(logIndex) = logMessage;

        logIndex = logIndex == maxLogMessage ? logIndex : logIndex+1;
    }
} //!namespace
