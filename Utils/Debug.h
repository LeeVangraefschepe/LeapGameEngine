#pragma once
#include <string>

namespace leap
{
	class Debug final
	{
	public:
		Debug() = delete;

		static void Log(const std::string& message) { Log(message.c_str()); }
		static void LogWarning(const std::string& message) { LogWarning(message.c_str()); }
		static void LogError(const std::string& message) { LogError(message.c_str()); }

		static void Log(const char* message);
		static void LogWarning(const char* message);
		static void LogError(const char* message);

		static void SetFileLogger(bool enabled);
		static void SetThrowingOnError(bool enabled);
	private:

		static bool m_IsFileLogging;
		static bool m_IsThrowingOnError;
		static std::string GetTime();
	};
}