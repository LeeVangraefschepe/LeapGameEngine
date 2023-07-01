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
		explicit Scene(std::string name);
		~Scene() = default;

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		GameObject* CreateGameObject(const std::string& name) const;
		void RemoveAll();
		const GameObject* GetRootObject() const;

	private:
		friend SceneManager;
		void OnFrameStart() const;
		void FixedUpdate() const;
		void Update() const;
		void LateUpdate() const;
		void Render() const;
		void OnGUI() const;
		void OnFrameEnd() const;

		std::string m_Name;
		std::unique_ptr<GameObject> m_RootObject;
	};
}