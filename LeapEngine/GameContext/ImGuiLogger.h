#pragma once
#include "Debug.h"
#include "Observer.h"
#include <vector>

namespace leap
{
	class ImGuiLogger final : Observer<Debug::LogInfo>
	{
	public:
		ImGuiLogger();
		virtual ~ImGuiLogger();
		ImGuiLogger(const ImGuiLogger& other) = delete;
		ImGuiLogger(ImGuiLogger&& other) = delete;
		ImGuiLogger& operator=(const ImGuiLogger& other) = delete;
		ImGuiLogger& operator=(ImGuiLogger&& other) = delete;

		void SetEnabled(bool enable);

	private:
		friend class GameContext;

		void OnGUI();
		void Notify(const Debug::LogInfo& data) override;

		bool m_Enabled{ true };
		std::vector<std::vector<Debug::LogInfo>> m_Logs{};

		Debug::Type m_ActiveType{ Debug::Type::Message };
	};
}