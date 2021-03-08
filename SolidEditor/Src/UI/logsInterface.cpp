#include "UI/logsInterface.hpp"
#include <imgui.h>

namespace Solid
{
    void LogsInterface::Draw()
    {
        if(!p_open)
            return;

        UI::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.3, 0.3, 0.3, 1.0));

        UI::SetNextWindowSize(ImVec2(250, 250));
        UI::Begin("Logs", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        if (!UI::IsWindowFocused())
        {
            UI::PopStyleColor();
            UI::End();
            return;
        }

        for (unsigned int i = maxLogMessage-1; i > 0; i--)
        {
            auto currLog = Log::GetLogAt(i);
            std::string currMsg = currLog.logMessage;
            if (currMsg.empty())
                continue;
            switch (currLog.logSeverity)
            {
                case Log::ELogSeverity::INFO:
                    currMsg = "[INFO] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.8, 1.0, 1.0));
                    break;
                case Log::ELogSeverity::DEBUG:
                    currMsg = "[DEBUG] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1.0));
                    break;
                case Log::ELogSeverity::WARNING:
                    currMsg = "[WARNING] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.9, 0.0, 1.0));
                    break;
                case Log::ELogSeverity::CRITICAL:
                    currMsg = "[CRITICAL] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.1, 0.1, 1.0));
                    break;
                case Log::ELogSeverity::ERROR:
                    currMsg = "[ERROR] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.1, 0.1, 1.0));
                    break;

            }
            UI::TextWrapped("%s", currMsg.c_str());
            UI::PopStyleColor(1);
        }
        UI::End();

        UI::PopStyleColor(1);
    }
}
