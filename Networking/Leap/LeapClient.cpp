#include "LeapClient.h"
#include <array>
#include <chrono>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

leap::networking::LeapClient::LeapClient(int port, const std::string& serverIp, int receiveBuffer, int packetBuffer) : m_bufferSize(receiveBuffer)
{
    //Initialize Winsock
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsData) != 0)
    {
        std::cerr << "Error initializing Winsock: " << WSAGetLastError() << std::endl;
        return;
    }

    //Create a socket for the client
    m_TCPsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_TCPsocket == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }
    m_UDPsocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_UDPsocket == INVALID_SOCKET)
    {
        perror("Error creating UDP socket");
        WSACleanup();
        return;
    }

    //Connect to the server
    m_pServerAdress = std::make_unique<sockaddr_in>();
    m_pServerAdress->sin_family = AF_INET;
    m_pServerAdress->sin_port = htons(static_cast<u_short>(port));  //Port number of the server
    inet_pton(AF_INET, serverIp.c_str(), &m_pServerAdress->sin_addr);  //IP address of the server

    if (connect(m_TCPsocket, reinterpret_cast<sockaddr*>(m_pServerAdress.get()), sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(m_TCPsocket);
        WSACleanup();
        return;
    }

    m_packetReceiver = std::make_unique<EventPool<BasePacket>>(packetBuffer);
    m_packetSender = std::make_unique<EventPool<InternalPacket>>(packetBuffer);

    m_sendThread = std::jthread{ &LeapClient::HandleSend, this };
}

leap::networking::LeapClient::~LeapClient()
{
    m_sendThread.request_stop();
    m_sendCondition.notify_one();
    closesocket(m_TCPsocket);
    closesocket(m_UDPsocket);
    WSACleanup();
}

bool leap::networking::LeapClient::GetPacket(BasePacket& packet)
{
    return m_packetReceiver->Get(packet);
}

void leap::networking::LeapClient::SendTCP(const BasePacket& packet)
{
    m_packetSender->Add(InternalPacket{ packet });
    m_sendCondition.notify_one();
}

void leap::networking::LeapClient::SendUDP(const BasePacket& packet)
{
    m_packetSender->Add(InternalPacket{ packet, true });
    m_sendCondition.notify_one();
}

void leap::networking::LeapClient::Run(float ticks)
{
    m_connected = true;
    m_clientThread = std::jthread{ [this, ticks]() { InternalRun(ticks); } };
    m_clientThread.detach();
}

bool leap::networking::LeapClient::IsConnected()
{
    return true;
}

void leap::networking::LeapClient::InternalRun(float ticks)
{
    const float tickTimeMs{ 1000 / ticks };
    bool connected{ true };
    while (connected)
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        connected = HandleReceive();

        const auto sleepTimeMs = tickTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }
}

bool leap::networking::LeapClient::HandleReceive()
{
    // Receive a response from the server
    std::vector<char> buffer{};
    buffer.resize(m_bufferSize);

    const int bytesReceived = recv(m_TCPsocket, buffer.data(), static_cast<int>(buffer.size()), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        closesocket(m_TCPsocket);
        WSACleanup();
        m_connected = false;
        return false;
    }
    if (bytesReceived == 0)
    {
        closesocket(m_TCPsocket);
        WSACleanup();
        m_connected = false;
        return false;
    }

    //Create packet core
    std::vector<char> charBuffer{ std::begin(buffer), std::end(buffer) };

    //WARNING PACKET CREATION WILL DELETE CHAR BUFFER
    BasePacket packet{};
    packet.SetData(charBuffer);
    m_packetReceiver->Add(packet);

    return true;
}

void leap::networking::LeapClient::HandleSend()
{
    const std::stop_token& stopToken{ m_sendThread.get_stop_token() };
    InternalPacket data{};

    while (!stopToken.stop_requested())
    {
        std::unique_lock lock{ m_mutex };
        m_sendCondition.wait(lock, [&]() { return m_packetSender->Get(data) || stopToken.stop_requested(); });
        lock.unlock();

        if (stopToken.stop_requested()) { break; }

        if (data.IsUDP)
        {
            const int bytesSentUDP = sendto(m_UDPsocket, data.Packet.GetData(), data.Packet.Length(), 0, reinterpret_cast<struct sockaddr*>(m_pServerAdress.get()), sizeof(sockaddr_in));
            if (bytesSentUDP == SOCKET_ERROR) perror("Error sending data to UDP server");
        }
        else
        {
            if (send(m_TCPsocket, data.Packet.GetData(), data.Packet.Length(), 0) == SOCKET_ERROR)
            {
                closesocket(m_TCPsocket);
                WSACleanup();
                m_connected = false;
            }
        }
    }
}