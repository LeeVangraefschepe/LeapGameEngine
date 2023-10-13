#pragma once
#include <string>
#include <source_location>

namespace leap
{
	class Debug final
	{
	public:
		Debug() = delete;

		static void Log(const std::string& message, const std::source_location& location = std::source_location::current()) { Log(message.c_str(), location); }
		static void LogWarning(const std::string& message, const std::source_location& location = std::source_location::current()) { LogWarning(message.c_str(), location); }
		static void LogError(const std::string& message, const std::source_location& location = std::source_location::current()) { LogError(message.c_str(), location); }

		static void Log(const char* message, const std::source_location& location = std::source_location::current());
		static void LogWarning(const char* message, const std::source_location& location = std::source_location::current());
		static void LogError(const char* message, const std::source_location& location = std::source_location::current());

		static void SetFileLogger(bool enabled);
		static void SetThrowingOnError(bool enabled);
	private:

		static bool m_IsFileLogging;
		static bool m_IsThrowingOnError;
		static std::string GetTime();
		static void PrintCallStack(const std::source_location& location);

#pragma region Color codes
		inline static const char* RESET = "\033[0m";         /* Default */
		inline static const char* BLACK = "\033[30m";        /* Black */
		inline static const char* RED = "\033[31m";        /* Red */
		inline static const char* GREEN = "\033[32m";        /* Green */
		inline static const char* YELLOW = "\033[33m";        /* Yellow */
		inline static const char* BLUE = "\033[34m";        /* Blue */
		inline static const char* MAGENTA = "\033[35m";        /* Magenta */
		inline static const char* CYAN = "\033[36m";        /* Cyan */
		inline static const char* WHITE = "\033[37m";        /* White */
		inline static const char* BOLDBLACK = "\033[1m\033[30m"; /* Bold Black */
		inline static const char* BOLDRED = "\033[1m\033[31m"; /* Bold Red */
		inline static const char* BOLDGREEN = "\033[1m\033[32m"; /* Bold Green */
		inline static const char* BOLDYELLOW = "\033[1m\033[33m"; /* Bold Yellow */
		inline static const char* BOLDBLUE = "\033[1m\033[34m"; /* Bold Blue */
		inline static const char* BOLDMAGENTA = "\033[1m\033[35m"; /* Bold Magenta */
		inline static const char* BOLDCYAN = "\033[1m\033[36m"; /* Bold Cyan */
		inline static const char* BOLDWHITE = "\033[1m\033[37m"; /* Bold White */
#pragma endregion 
	};
}