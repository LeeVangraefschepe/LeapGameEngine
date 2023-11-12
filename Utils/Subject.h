#pragma once

#include "Observer.h"

#include <vector>
#include <functional>

namespace leap
{
	class Subject
	{
	public:
		Subject() = default;
		~Subject()
		{
			for (auto& observer : m_Observers)
			{
				observer->OnSubjectDestroy();
			}
		}

		void AddListener(Observer* observer)
		{
			m_Observers.push_back(observer);
		}
		void AddListener(void(*pCallback)())
		{
			m_Functions.push_back(pCallback);
		}
		void RemoveListener(Observer* observer)
		{
			const auto& it{ std::find(begin(m_Observers), end(m_Observers), observer) };
			if (it != end(m_Observers)) m_Observers.erase(it);
		}
		void RemoveListener(void(*pCallback)())
		{
			const auto& it{ std::find(begin(m_Functions), end(m_Functions), pCallback) };
			if (it != end(m_Functions)) m_Functions.erase(it);
		}

		void Notify()
		{
			for (const auto& observer : m_Observers)
			{
				observer->Notify();
			}
			for (const auto& function : m_Functions)
			{
				function();
			}
		}
	private:
		std::vector<Observer*> m_Observers{};
		std::vector<void(*)()> m_Functions{};
	};

	template <class T>
	class TSubject
	{
	public:
		TSubject() = default;
		~TSubject()
		{
			for (auto& observer : m_Observers)
			{
				observer->OnSubjectDestroy();
			}
		}

		void AddListener(TObserver<T>* observer)
		{
			m_Observers.push_back(observer);
		}
		void AddListener(void(*pCallback)(const T&))
		{
			m_Functions.push_back(pCallback);
		}
		void RemoveListener(TObserver<T>* observer)
		{
			const auto& it{ std::find(begin(m_Observers), end(m_Observers), observer) };
			if (it != end(m_Observers)) m_Observers.erase(it);
		}
		void RemoveListener(void(*pCallback)(const T&))
		{
			const auto& it{ std::find(begin(m_Functions), end(m_Functions), pCallback) };
			if (it != end(m_Functions)) m_Functions.erase(it);
		}

		void Notify(const T& data)
		{
			for (const auto& observer : m_Observers)
			{
				observer->Notify(data);
			}
			for (const auto& function : m_Functions)
			{
				function(data);
			}
		}
	private:
		std::vector<TObserver<T>*> m_Observers{};
		std::vector<void(*)(const T&)> m_Functions{};
	};
}