#pragma once
#include <string>
#include <memory>

#include "GameObject.h"

namespace leap
{
	class SceneManager;
	class Scene final
	{
	public:
		explicit Scene(const char* name);
		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		GameObject* CreateGameObject(const char* name) const;
		void RemoveAll();
		GameObject* GetRootObject() const;

	private:
		friend SceneManager;
		void OnFrameStart() const;
		void FixedUpdate() const;
		void Update() const;
		void LateUpdate() const;
		void OnGUI() const;
		void OnFrameEnd() const;

		std::unique_ptr<GameObject> m_pRootObject{};
	};
}