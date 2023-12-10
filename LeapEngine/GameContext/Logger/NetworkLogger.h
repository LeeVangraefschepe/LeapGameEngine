#pragma once
#include "Debug.h"
#include "ILogger.h"
#include "Leap/LeapClient.h"

namespace leap
{
	class NetworkLogger final : public ILogger
	{
	public:
		NetworkLogger();
		virtual ~NetworkLogger();
		NetworkLogger(const NetworkLogger& other) = delete;
		NetworkLogger(NetworkLogger&& other) = delete;
		NetworkLogger& operator=(const NetworkLogger& other) = delete;
		NetworkLogger& operator=(NetworkLogger&& other) = delete;

		virtual void SetEnabled(bool enable) override;

	private:
		void OnDebug(const Debug::LogInfo& data) ;
		bool m_Enabled{};

		inline static constexpr int MAX_BUFFER = 1024;
		networking::LeapClient m_Client{ 1234, "127.0.0.1", MAX_BUFFER, 30 };
	};
}