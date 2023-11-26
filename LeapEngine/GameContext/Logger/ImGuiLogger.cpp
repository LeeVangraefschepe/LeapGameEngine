#include "ImGuiLogger.h"
#include "ImGui/imgui.h"
#include <sstream>

leap::ImGuiLogger::ImGuiLogger()
{
    // Create vectors for type of logging
    m_Logs.emplace_back();
    m_Logs.emplace_back();
    m_Logs.emplace_back();

    SetEnabled(true);
}

leap::ImGuiLogger::~ImGuiLogger()
{
    Debug::OnEvent.RemoveListener(this, &ImGuiLogger::OnDebug);
}

void leap::ImGuiLogger::SetEnabled(bool enable)
{
    if (m_Enabled == enable) return;

    if (enable)
    {
        Debug::OnEvent.AddListener(this, &ImGuiLogger::OnDebug);
    }
    else
    {
        Debug::OnEvent.RemoveListener(this, &ImGuiLogger::OnDebug);
    }
    m_Enabled = enable;
}

void leap::ImGuiLogger::OnGUI()
{
    if (!m_Enabled) return;

    // Create a window
    ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

    // Create the tab bar
    if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Messages")) {
            m_ActiveType = Debug::Type::Message;
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Warnings")) {
            m_ActiveType = Debug::Type::Warning;
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Errors")) {
            m_ActiveType = Debug::Type::Error;
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Clear")) {
            m_Logs[static_cast<int>(m_ActiveType)].clear();
            ImGui::EndTabItem();
            ImGui::SetTabItemClosed("Clear");
        }

        if (ImGui::BeginTabItem("Close")) {
            ImGui::EndTabItem();
            ImGui::SetTabItemClosed("Close");
            SetEnabled(false);
        }

        // End the tab bar
        ImGui::EndTabBar();
    }

    for (const auto& data : m_Logs[static_cast<int>(m_ActiveType)])
    {
        std::stringstream ss{};
        ss << data.Time << ' ' << data.Message << '\n';
        ss << "** " << data.Location.file_name() << " line " << data.Location.line() << ":" << data.Location.column() << '\n';
        ImGui::Text(ss.str().c_str());
        ImGui::Spacing();
    }

    // End the main window
    ImGui::End();
}

void leap::ImGuiLogger::OnDebug(const Debug::LogInfo& message)
{
    LogInfo log{};
    log.Message = std::string{ message.Message };
    log.Time = std::string{ message.Time };
    log.Location = message.Location;

    m_Logs[static_cast<int>(message.Type)].push_back(log);
}