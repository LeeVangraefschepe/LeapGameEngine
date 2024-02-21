#pragma once

#include <algorithm>
#include <vector>
#include <functional>
#include <memory>

#include "ReflectionUtils.h"

namespace leap
{
	class Subject
	{
	private:
		struct CallbackData final
		{
			const void* pTarget;
			std::function<void()> Callback;
		};

	public:
		template<typename T>
		void AddListener(T* const pTarget, void(T::* pFunction)())
		{
			m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget));
		}

		template<typename T>
		void AddListener(const T* const pTarget, void(T::* pFunction)() const)
		{
			m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget));
		}

		void AddListener(void(*pFunction)())
		{
			m_Callbacks.emplace_back(nullptr, pFunction);
		}

		void RemoveListener(void* const pTarget)
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pTarget](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == pTarget;
				}), m_Callbacks.end());
		}

		void RemoveListener(void(*pFunction)())
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pFunction, this](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == nullptr && GetFunctionAddress(callbackData.Callback) == reinterpret_cast<uint64_t>(pFunction);
				}), m_Callbacks.end());
		}

		void Notify() const
		{
			for (const CallbackData& callbackData : m_Callbacks)
			{
				callbackData.Callback();
			}
		}

	private:
		uint64_t GetFunctionAddress(const std::function<void()>& function) const
		{
			typedef void(FunctionType)();
			FunctionType* const* functionPointer{ function.template target<FunctionType*>() };

			return reinterpret_cast<uint64_t>(functionPointer);
		}

		std::vector<CallbackData> m_Callbacks;
	};

	template <typename ... Ts>
	class TSubject
	{
	private:
		struct CallbackData final
		{
			const void* pTarget;
			std::function<void(Ts...)> Callback;
		};

		inline constexpr static int m_NrOfArgs = sizeof ... (Ts);

	public:
		template<typename T>
		void AddListener(T* const pTarget, void(T::* pFunction)(Ts...))
		{
			AddListenerInternal(pTarget, pFunction, std::make_index_sequence<m_NrOfArgs>{});
		}

		template<typename T>
		void AddListener(const T* const pTarget, void(T::* pFunction)(Ts...) const)
		{
			AddListenerInternal(pTarget, pFunction, std::make_index_sequence<m_NrOfArgs>{});
		}

		void AddListener(void(*pFunction)(Ts...))
		{
			AddListenerInternal(pFunction);
		}

		void RemoveListener(void* const pTarget)
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pTarget](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == pTarget;
				}), m_Callbacks.end());
		}

		void RemoveListener(void(*pFunction)(Ts...))
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pFunction, this](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == nullptr && GetFunctionAddress(callbackData.Callback) == reinterpret_cast<uint64_t>(pFunction);
				}), m_Callbacks.end());
		}

		template<typename ... Us>
		void Notify(Us&& ... args) const
		{
			static_assert(sizeof ... (Us) == m_NrOfArgs, "Can only call Notify() with an equal number of arguments");

			for (const CallbackData& callbackData : m_Callbacks)
			{
				callbackData.Callback(std::forward<Us>(args)...);
			}
		}

	private:
		template<typename T, size_t ... Is>
		void AddListenerInternal(T* const pTarget, void(T::* pFunction)(Ts...), const std::index_sequence<Is...>)
		{
			if constexpr (m_NrOfArgs == 0)
			{
				m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget));
			}
			else
			{
				m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget, std::_Ph<Is + 1>{})...);
			}
		}

		template<typename T, size_t ... Is>
		void AddListenerInternal(const T* const pTarget, void(T::* pFunction)(Ts...) const, const std::index_sequence<Is...>)
		{
			if constexpr (m_NrOfArgs == 0)
			{
				m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget));
			}
			else
			{
				m_Callbacks.emplace_back(pTarget, std::bind(pFunction, pTarget, std::_Ph<Is + 1>{})...);
			}
		}

		void AddListenerInternal(void(*pFunction)(Ts...))
		{
			m_Callbacks.emplace_back(nullptr, pFunction);
		}

		uint64_t GetFunctionAddress(const std::function<void(Ts...)>& function) const
		{
			typedef void(FunctionType)(Ts...);
			FunctionType* const* functionPointer{ function.template target<FunctionType*>() };

			return reinterpret_cast<uint64_t>(functionPointer);
		}

		std::vector<CallbackData> m_Callbacks;
	};
}