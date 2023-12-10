#include "ConsoleLogger.h"
#include <iostream>

leap::ConsoleLogger::ConsoleLogger()
{
    SetEnabled(true);
}

leap::ConsoleLogger::~ConsoleLogger()
{
    Debug::OnEvent.RemoveListener(this, &ConsoleLogger::OnDebug);
}

void leap::ConsoleLogger::SetEnabled(bool enable)
{
    if (m_Enabled == enable) return;

    if (enable)
    {
        Debug::OnEvent.AddListener(this, &ConsoleLogger::OnDebug);
    }
    else
    {
        Debug::OnEvent.RemoveListener(this, &ConsoleLogger::OnDebug);
    }
    m_Enabled = enable;
}

void leap::ConsoleLogger::OnDebug(const Debug::LogInfo& data) const
{
    switch (data.Type)
    {
    case Debug::Type::Message:
        std::cout << BOLDWHITE;
        break;
    case Debug::Type::Warning:
        std::cout << BOLDYELLOW;
        break;
    case Debug::Type::Error:
        std::cout << BOLDRED;
        break;
    }

    std::cout << data.Time << ' ' << data.Message << '\n';
    std::cout << "** " << data.Location.file_name() << " line " << data.Location.line() << ":" << data.Location.column() << '\n' << RESET << '\n';
}
