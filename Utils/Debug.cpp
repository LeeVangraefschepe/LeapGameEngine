#include "Debug.h"
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <windows.h>

bool leap::Debug::m_IsFileLogging { false };
bool leap::Debug::m_IsThrowingOnError { true };

void leap::Debug::Log(const char* message, const std::source_location& location)
{
	const auto time = GetTime();
	std::cout << BOLDWHITE << time << ' ' << message << '\n';
    PrintCallStack(location);
    std::cout << RESET << '\n';
}

void leap::Debug::LogWarning(const char* message, const std::source_location& location)
{
    const auto time = GetTime();
    std::cout << BOLDYELLOW << time << ' ' << message << '\n';
    PrintCallStack(location);
    std::cout << RESET << '\n';
}

void leap::Debug::LogError(const char* message, const std::source_location& location)
{
    const auto time = GetTime();
    std::cout << BOLDRED << time << ' ' << message << '\n';
    PrintCallStack(location);
    std::cout << RESET << '\n';

    if (!m_IsThrowingOnError) return;

    throw std::runtime_error{ message };
}

void leap::Debug::SetFileLogger(bool enabled)
{
	m_IsFileLogging = enabled;
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

void leap::Debug::PrintCallStack(const std::source_location& location)
{
    std::cout << "** " << location.file_name() << " line " << location.line() << ":" << location.column() << '\n';
}
