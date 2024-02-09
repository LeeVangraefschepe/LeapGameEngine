#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "Scene.h"
#include "Singleton.h"
#include "../Coroutines/CoroutineSystem.h"

namespace leap
{
	enum class EngineExecutionState
	{
		OnFrameStart,
		FixedUpdate,
		Update,
		LateUpdate,
		OnFrameEnd
	};

	class LeapEngine;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager() override = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		Scene* GetActiveScene() const;
		void AddScene(const char* name, const std::function<void(Scene&)>& load);
		void LoadScene(unsigned index);
		void LoadScene(const std::string& name);

		CoroutineSystem* GetCoroutineSystem() const { return m_pCoroutineSystem.get(); }
		EngineExecutionState GetEngineExecutionState() const { return m_EngineExecutionState; }

	private:
		friend LeapEngine;
		void OnFrameStart();
		void FixedUpdate() const;
		void Update() const;
		void LateUpdate() const;
		void OnGUI() const;
		void OnFrameEnd() const;

		void LoadInternalScene();
		void UnloadScene();

		void SetEngineExecutionState(EngineExecutionState state);

		struct SceneData final
		{
			const std::string name;
			std::function<void(Scene&)> load;
		};
		EngineExecutionState m_EngineExecutionState;
		std::vector<SceneData> m_Scenes{};
		std::unique_ptr<Scene> m_Scene{};
		std::unique_ptr<CoroutineSystem> m_pCoroutineSystem{};
		int m_LoadScene{ -1 };
	};
}