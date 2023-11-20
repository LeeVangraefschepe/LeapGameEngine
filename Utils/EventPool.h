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
		std::mutex m_lockData{};
		std::vector<T> m_vectorData{};
		int m_packetAmount{};
	};

	template <typename T>
	EventPool<T>::EventPool(int bufferSize)
	{
		std::unique_lock lock{ m_lockData };
		m_vectorData.resize(bufferSize);
	}

	template <typename T>
	bool EventPool<T>::Get(T& data)
	{
		std::unique_lock lock{ m_lockData };
		if (m_packetAmount < 1) { return false; }
		data = m_vectorData[--m_packetAmount];
		return true;
	}

	template <typename T>
	bool EventPool<T>::Add(T data)
	{
		std::unique_lock lock{ m_lockData };
		if (m_packetAmount >= static_cast<int>(m_vectorData.size()) - 1)
		{
			Debug::LogWarning("Data is not saved. Increase buffer size");
			return true;
		}

		m_vectorData[m_packetAmount++] = data;
		return false;
	}
}
