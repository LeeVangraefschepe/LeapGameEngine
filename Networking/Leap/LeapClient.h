#pragma once
#include <map>
#include <string>
#include <thread>
#include <condition_variable>
#include "../Interfaces/IClient.h"
#include "../Interfaces/BasePacket.h"
#include "EventPool.h"

struct sockaddr_in;

namespace leap::networking
{
	class LeapClient : public IClient
	{
	public:
		LeapClient(int port, const std::string& serverIp, int receiveBuffer = 256, int packetBuffer = 25);
		virtual ~LeapClient();
		LeapClient(const LeapClient& other) = delete;
		LeapClient(LeapClient&& other) = delete;
		LeapClient& operator=(const LeapClient& other) = delete;
		LeapClient& operator=(LeapClient&& other) = delete;

		bool GetPacket(BasePacket& packet) override;
		void SendTCP(const BasePacket& packet) override;
		void SendUDP(const BasePacket& packet) override;
		void Run(float ticks) override;
		bool IsConnected() override;

	private:
		struct InternalPacket final
		{
			BasePacket Packet;
			bool IsUDP{};
		};

		void InternalRun(float ticks);
		bool HandleReceive();
		void HandleSend();

		std::jthread m_clientThread{};
		std::jthread m_sendThread{};
		std::condition_variable m_sendCondition{};

		int m_bufferSize;

		bool m_connected{ false };
		unsigned long long m_TCPsocket{};
		unsigned long long m_UDPsocket{};
		std::unique_ptr<sockaddr_in> m_pServerAdress;

		std::unique_ptr<EventPool<BasePacket>> m_packetReceiver;
		std::unique_ptr<EventPool<InternalPacket>> m_packetSender;

		std::mutex m_mutex{};
	};
}
