#pragma once
#include <string>
#include "Debug.h"
#include <vector>
#include "ILogger.h"

namespace leap
{
	class ImGuiLogger final : public ILogger
	{
	public:
		ImGuiLogger();
		virtual ~ImGuiLogger();
		ImGuiLogger(const ImGuiLogger& other) = delete;
		ImGuiLogger(ImGuiLogger&& other) = delete;
		ImGuiLogger& operator=(const ImGuiLogger& other) = delete;
		ImGuiLogger& operator=(ImGuiLogger&& other) = delete;

		virtual void SetEnabled(bool enable) override;

	private:
		struct LogInfo final
		{
			std::string Message;
			std::string Time;
			std::source_location Location;
		};

		virtual void OnGUI() override;
		void OnDebug(const Debug::LogInfo& data);

		bool m_Enabled{};
		std::vector<std::vector<LogInfo>> m_Logs{};

		Debug::Type m_ActiveType{ Debug::Type::Message };
	};
}