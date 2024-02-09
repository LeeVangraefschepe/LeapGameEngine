#pragma once
#include "Singleton.h"
#include <memory>
#include <vector>
#include "Logger/ILogger.h"

class GLFWwindow;

namespace leap
{
	class LeapEngine;
	class Timer;
	class Window;

	class GameContext final : public Singleton<GameContext>
	{
	public:
		virtual ~GameContext();
		GameContext(const GameContext& other) = delete;
		GameContext(GameContext&& other) = delete;
		GameContext& operator=(const GameContext& other) = delete;
		GameContext& operator=(GameContext&& other) = delete;

		Timer* GetTimer() const { return m_pTimer.get(); }
		Window* GetWindow() const { return m_pWindow.get(); }

		template <class T>
		T* AddLogger();
		template <class T>
		T* GetLogger() const;
	private:
		friend Singleton;
		friend LeapEngine;
		GameContext();

		void Update();
		void OnGUI();
		void CreateWindowWrapper(GLFWwindow* window);

		std::unique_ptr<Timer> m_pTimer;
		std::unique_ptr<Window> m_pWindow;

		std::vector<std::unique_ptr<ILogger>> m_pLoggers{};
	};

	template<class T>
	inline T* GameContext::GetLogger() const
	{
		static_assert(std::is_base_of<ILogger, T>::value, "T needs to be derived from the ILogger class");

		const auto iterator{ std::find_if(begin(m_pLoggers), end(m_pLoggers), [](const auto& pComponent) { return dynamic_cast<T*>(pComponent.get()) != nullptr; }) };

		if (iterator == end(m_pLoggers)) return nullptr;

		return static_cast<T*>(iterator->get());
	}
	template <class T>
	T* GameContext::AddLogger()
	{
		static_assert(std::is_base_of<ILogger, T>::value, "T needs to be derived from the ILogger class");

		// Create a new logger
		auto pLogger{ std::make_unique<T>() };

		// Get the raw ptr from the new logger
		T* pRawComponent{ pLogger.get() };

		// Add the new logger to the list
		m_pLoggers.emplace_back(std::move(pLogger));

		// Return the raw ptr of the new logger
		return pRawComponent;
	}

}