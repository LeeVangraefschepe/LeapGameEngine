#include "Debug.h"
#include <ctime>
#include <stdexcept>

void leap::Debug::Log(const char* message, const std::source_location& location)
{
	const auto time = GetTime();
    OnEvent.Notify(LogInfo{ message, GetTime().c_str(), Type::Message, location });
}

void leap::Debug::LogWarning(const char* message, const std::source_location& location)
{
    const auto time = GetTime();
    OnEvent.Notify(LogInfo{ message, GetTime().c_str(), Type::Warning, location });
}

void leap::Debug::LogError(const char* message, const std::source_location& location)
{
    const auto time = GetTime();
    OnEvent.Notify(LogInfo{ message, GetTime().c_str(), Type::Error, location });

    if (!m_IsThrowingOnError) return;

    throw std::runtime_error{ message };
}

void leap::Debug::SetThrowingOnError(bool enabled)
{
    m_IsThrowingOnError = enabled;
}

std::string leap::Debug::GetTime()
{
    time_t currentTime;
    tm timeinfo{};
    time(&currentTime);
    localtime_s(&timeinfo, &currentTime);
    char timeString[11]; // [hh:mm:ss] plus the null terminator
    strftime(timeString, sizeof(timeString), "[%H:%M:%S]", &timeinfo);

    return std::string{timeString};
}