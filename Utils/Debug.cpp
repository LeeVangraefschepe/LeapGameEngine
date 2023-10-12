#include "Debug.h"
#include <iostream>
#include <ctime>
#include <stdexcept>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

bool leap::Debug::m_IsFileLogging { false };
bool leap::Debug::m_IsThrowingOnError { true };

void leap::Debug::Log(const char* message)
{
	const auto time = GetTime();
	std::cout << BOLDWHITE << time << ' ' << message << '\n' << RESET;
}

void leap::Debug::LogWarning(const char* message)
{
    const auto time = GetTime();
    std::cout << BOLDYELLOW << time << ' ' << message << '\n' << RESET;
}

void leap::Debug::LogError(const char* message)
{
    const auto time = GetTime();
    std::cout << BOLDRED << time << ' ' << message << '\n' << RESET;

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