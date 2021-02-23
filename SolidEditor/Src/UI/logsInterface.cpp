#include "UI/logsInterface.hpp"
#include <imgui.h>

namespace Solid
{
    void LogsInterface::Draw()
    {

        UI::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.4, 0.4, 0.4, 1.0));

        UI::SetNextWindowSize(ImVec2(250, 250));
        UI::Begin("Logs", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        if (!UI::IsWindowFocused())
        {
            UI::PopStyleColor();
            UI::End();
            return;
        }
        UI::TextWrapped("Welcome on the Solid Play Scene!");

        for (unsigned int i = maxLogMessage-1; i > 0; i--)
        {
            auto currLog = Log::GetLogAt(i);
            std::string currMsg = currLog.logMessage;
            if (currMsg == "")
                continue;
            switch (currLog.logSeverity)
            {
                case Log::ELogSeverity::INFO:
                    currMsg = "[INFO] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.8, 1.0, 1.0));
                    break;
                case Log::ELogSeverity::DEBUG:
                    currMsg = "[DEBUG] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1.0));
                    break;
                case Log::ELogSeverity::WARNING:
                    currMsg = "[WARNING] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.9, 0.0, 1.0));
                    break;
                default:
                    currMsg = "[CRITICAL] " + currMsg;
                    UI::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.1, 0.1, 1.0));
                    break;
            }
            UI::Text("%s", currMsg.c_str());
            UI::PopStyleColor(1);
        }
        UI::End();

        UI::PopStyleColor(1);
    }
}
