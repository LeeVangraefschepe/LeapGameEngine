#pragma once
#include <string>
#include <source_location>
#include "Subject.h"

namespace leap
{
	class Debug final
	{
	public:
		Debug() = delete;

		enum class Type
		{
			Message,
			Warning,
			Error
		};
		struct LogInfo
		{
			const char* Message;
			const char* Time;
			Type Type;
			std::source_location Location;
		};

		static void Log(const std::string& message, const std::source_location& location = std::source_location::current()) { Log(message.c_str(), location); }
		static void LogWarning(const std::string& message, const std::source_location& location = std::source_location::current()) { LogWarning(message.c_str(), location); }
		static void LogError(const std::string& message, const std::source_location& location = std::source_location::current()) { LogError(message.c_str(), location); }

		static void Log(const char* message, const std::source_location& location = std::source_location::current());
		static void LogWarning(const char* message, const std::source_location& location = std::source_location::current());
		static void LogError(const char* message, const std::source_location& location = std::source_location::current());

		static void SetThrowingOnError(bool enabled);

		inline static TSubject<LogInfo> OnEvent{};

	private:
		static std::string GetTime();
		static inline bool m_IsThrowingOnError{true};
	};
}
