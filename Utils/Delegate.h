#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace leap
{
	template<typename ... Ts>
	class Delegate
	{
		struct CallbackData final
		{
			const void* pTarget;
			std::function<void(Ts...)> Callback;
		};

		inline constexpr static int m_NrOfArgs = sizeof ... (Ts);

	public:
		template<typename T>
		void Bind(T* const pTarget, void(T::* pFunction)(Ts...))
		{
			BindInternal(pTarget, pFunction, std::make_index_sequence<m_NrOfArgs>{});
		}

		template<typename T>
		void Bind(const T* const pTarget, void(T::* pFunction)(Ts...) const)
		{
			BindInternal(pTarget, pFunction, std::make_index_sequence<m_NrOfArgs>{});
		}

		void Bind(void(*pFunction)(Ts...))
		{
			BindInternal(pFunction);
		}

		void Unbind(void* const pTarget)
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pTarget](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == pTarget;
				}), m_Callbacks.end());
		}

		void Unbind(void(*pFunction)(Ts...))
		{
			m_Callbacks.erase(std::remove_if(m_Callbacks.begin(), m_Callbacks.end(),
				[pFunction](const CallbackData& callbackData)->bool
				{
					return callbackData.pTarget == nullptr && callbackData.pFunction == pFunction;
				}), m_Callbacks.end());
		}

		template<typename ... Us>
		void Invoke(Us&& ... args) const
		{
			static_assert(sizeof ... (Us) == m_NrOfArgs, "Can only call Invoke() with an equal number of arguments");

			for (const CallbackData& callbackData : m_Callbacks)
			{
				callbackData.Callback(std::forward<Us>(args)...);
			}
		}

	private:
		template<typename T, size_t ... Is>
		void BindInternal(T* const pTarget, void(T::* pFunction)(Ts...), const std::index_sequence<Is...>)
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
		void BindInternal(const T* const pTarget, void(T::* pFunction)(Ts...) const, const std::index_sequence<Is...>)
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

		void BindInternal(void(* pFunction)(Ts...))
		{
			m_Callbacks.emplace_back(nullptr, pFunction);
		}

		std::vector<CallbackData> m_Callbacks;
	};
}

#define DECLARE_DELEGATE(Typename) using Typename = leap::Delegate<>
#define DECLARE_DELEGATE_OneParam(Typename, T1) using Typename = leap::Delegate<T1>
#define DECLARE_DELEGATE_TwoParams(Typename, T1, T2) using Typename = leap::Delegate<T1, T2>
#define DECLARE_DELEGATE_ThreeParams(Typename, T1, T2, T3) using Typename = leap::Delegate<T1, T2, T3>
#define DECLARE_DELEGATE_FourParams(Typename, T1, T2, T3, T4) using Typename = leap::Delegate<T1, T2, T3, T4>
// Add more as necessary