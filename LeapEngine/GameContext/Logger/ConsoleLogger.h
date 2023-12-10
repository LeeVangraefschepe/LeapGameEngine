#pragma once
#include "Debug.h"
#include "ILogger.h"
namespace leap
{
	class ConsoleLogger final : public ILogger
	{
	public:
		ConsoleLogger();
		virtual ~ConsoleLogger();
		ConsoleLogger(const ConsoleLogger& other) = delete;
		ConsoleLogger(ConsoleLogger&& other) = delete;
		ConsoleLogger& operator=(const ConsoleLogger& other) = delete;
		ConsoleLogger& operator=(ConsoleLogger&& other) = delete;

		virtual void SetEnabled(bool enable) override;

	private:
		void OnDebug(const Debug::LogInfo& data) const;
		bool m_Enabled{};

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