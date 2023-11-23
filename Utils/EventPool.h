#pragma once
#include <mutex>
#include <vector>
#include "Debug.h"

namespace leap
{
	template <typename T>
	class EventPool final
	{
	public:
		EventPool(int bufferSize = 1);
		~EventPool() = default;
		EventPool(const EventPool& other) = delete;
		EventPool(EventPool&& other) = delete;
		EventPool& operator=(const EventPool& other) = delete;
		EventPool& operator=(EventPool&& other) = delete;

		bool Get(T& data);
		bool Add(T data);

	private:
		std::mutex m_LockData{};
		std::vector<T> m_VectorData{};
		int m_PacketAmount{};
	};

	template <typename T>
	EventPool<T>::EventPool(int bufferSize)
	{
		std::unique_lock lock{ m_LockData };
		m_VectorData.resize(bufferSize);
	}

	template <typename T>
	bool EventPool<T>::Get(T& data)
	{
		std::unique_lock lock{ m_LockData };
		if (m_PacketAmount < 1) { return false; }
		data = m_VectorData[--m_PacketAmount];
		return true;
	}

	template <typename T>
	bool EventPool<T>::Add(T data)
	{
		std::unique_lock lock{ m_LockData };
		if (m_PacketAmount >= static_cast<int>(m_VectorData.size()) - 1)
		{
			Debug::LogWarning("Data is not saved. Increase buffer size");
			return true;
		}

		m_VectorData[m_PacketAmount++] = data;
		return false;
	}
}
