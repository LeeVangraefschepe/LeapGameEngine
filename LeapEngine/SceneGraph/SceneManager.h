#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "Scene.h"
#include "Singleton.h"

namespace leap
{
	class LeapEngine;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;
		~SceneManager() override = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		Scene* GetActiveScene() const;
		void AddScene(const char* name, const std::function<void(Scene&)>& load);
		void LoadScene(unsigned index);
		void LoadScene(const std::string& name);

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

		struct SceneData final
		{
			const std::string name;
			std::function<void(Scene&)> load;
		};
		std::vector<SceneData> m_Scenes{};
		std::unique_ptr<Scene> m_Scene{};
		int m_LoadScene{ -1 };
	};
}