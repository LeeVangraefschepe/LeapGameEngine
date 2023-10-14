#include "ImGuiLogger.h"
#include "ImGui/imgui.h"

leap::ImGuiLogger::ImGuiLogger()
{
    Debug::OnEvent.AddListener(this);

    // Create vectors for type of logging
    m_Logs.emplace_back();
    m_Logs.emplace_back();
    m_Logs.emplace_back();
}

leap::ImGuiLogger::~ImGuiLogger()
{
    Debug::OnEvent.RemoveListener(this);
}

void leap::ImGuiLogger::SetEnabled(bool enable)
{
    if (m_Enabled == enable) return;

    if (enable)
    {
        Debug::OnEvent.AddListener(this);
    }
    else
    {
        Debug::OnEvent.RemoveListener(this);
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
            m_Logs[0].clear();
            m_Logs[1].clear();
            m_Logs[2].clear();
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


    for (const auto& message : m_Logs[static_cast<int>(m_ActiveType)])
    {
        ImGui::Text(message.Message);
    }

    // End the main window
    ImGui::End();
}

void leap::ImGuiLogger::Notify(const Debug::LogInfo& message)
{
    m_Logs[static_cast<int>(message.Type)].push_back(message);
}