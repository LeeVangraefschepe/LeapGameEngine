#pragma once

#include "../Components/Component.h"

#include <string>
#include <memory>
#include <vector>

namespace leap
{
	class Transform;
	class Scene;

	class GameObject final
	{
	public:
		GameObject(const char* name);
		~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void SetParent(GameObject* pParent);
		GameObject* GetParent() const { return m_pParent; };

		GameObject* CreateChild(const char* name);
		GameObject* GetChild(int index) const;
		size_t GetChildCount() const { return m_pChildren.size(); };

		void SetName(const char* name) { m_Name = name; }
		const char* GetName() const { return m_Name; };

		void SetTag(const char* tag) { m_Tag = tag; }
		const char* GetTag() const { return m_Tag; }

		void SetActive(bool isActive);
		bool IsActive() const;

		void Destroy();
		bool IsMarkedAsDead() const;

		template <class T>
		T* AddComponent();
		template <class T>
		bool HasComponent() const;
		template <class T>
		T* GetComponent() const;
		template <class T>
		std::vector<T*> GetComponents() const;
		template <class T>
		void RemoveComponent();
		template <class T>
		void RemoveComponent(T* pComponent);

		Transform* GetTransform() const;
	private:
		friend Scene;

		void OnEnable() const;
		void OnDisable() const;
		void Update() const;
		void FixedUpdate() const;
		void LateUpdate() const;
		void OnGUI() const;
		void OnDestroy() const;

		const char* GetRawName() const { return m_Name; };

		/// <summary>
		/// Internally used to initialize gameobjects/components and update their active state
		/// This will call the Awake, Start, OnEnable and OnDisable methods
		/// This is not leaked to components
		/// </summary>
		void OnFrameStart();
		void MoveNewObjectsAndComponents();
		void CallAwake() const;
		void ChangeActiveState();
		void SetWorldState(bool isActive);
		void CallStart() const;
		void CallEnableAndDisable() const;

		/// <summary>
		/// Internally used to remove gameobjects/components
		/// This will call OnDestroy methods
		/// This is not leaked to components
		/// </summary>
		void OnFrameEnd();
		void CheckDestroyFlag() const;
		void UpdateCleanup();

		/// <summary>
		/// These functions are internally used to 
		/// change and check values of the m_StateFlags bitmask
		/// </summary>
		bool IsActiveLocalNextFrame() const;
		bool IsActiveLocal() const;
		void SetActiveLocal(bool isActive);
		bool IsActiveWorld() const;
		void SetActiveWorld(bool isActive);

		enum class StateFlags : char
		{
			IsActiveLocalNextFrame	= 1 << 0,
			IsActiveLocal			= 1 << 1,
			IsActiveWorld			= 1 << 2,
			IsMarkedAsDead			= 1 << 3
		};

		unsigned char m_StateFlags{ static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame) };

		const char* m_Name{};
		const char* m_Tag{};

		GameObject* m_pParent{};
		Transform* m_pTransform{};

		std::vector<std::unique_ptr<GameObject>> m_pChildrenToAdd{};
		std::vector<std::unique_ptr<GameObject>> m_pChildren{};

		std::vector<std::unique_ptr<Component>> m_pComponentsToAdd{};
		std::vector<std::unique_ptr<Component>> m_pComponents{};
	};

	template<class T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T needs to be derived from the Component class");

		// TODO: Don't let the user add multiple Transform components

		// Create a new component and set its variables
		auto pComponent{ std::make_unique<T>() };
		pComponent->SetOwner(this);

		// Get the raw ptr from the new component
		T* pRawComponent{ pComponent.get() };

		// Add the new component to the container for components of the next frame
		m_pComponentsToAdd.emplace_back(std::move(pComponent));

		// Return the raw ptr of the new component
		return pRawComponent;
	}

	template<class T>
	inline bool GameObject::HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}

	template<class T>
	inline T* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>::value, "T needs to be derived from the Component class");

		const auto iterator{ std::find_if(begin(m_pComponents), end(m_pComponents), [](const auto& pComponent) { return dynamic_cast<T*>(pComponent.get()) != nullptr; }) };

		if (iterator == end(m_pComponents)) return nullptr;

		return static_cast<T*>(iterator->get());
	}

	template<class T>
	inline std::vector<T*> GameObject::GetComponents() const
	{
		static_assert(std::is_base_of<Component, T>::value, "T needs to be derived from the Component class");

		std::vector<T*> pComponents{};

		for (const auto& pComponent : m_pComponents)
		{
			if (dynamic_cast<T*>(pComponent.get()) != nullptr) pComponents.emplace_back(pComponent.get());
		}

		return pComponents;
	}

	template<class T>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T needs to be derived from the Component class");

		// TODO: Don't let the user destroy the transform component

		GetComponent<T>()->Destroy();
	}

	template<class T>
	inline void GameObject::RemoveComponent(T* pComponent)
	{
		static_assert(std::is_base_of<Component, T>::value, "T needs to be derived from the Component class");

		// TODO: Don't let the user destroy the transform component

		// Don't try to remove a component that is not on this gameobject
		if (std::find_if(begin(m_pComponents), end(m_pComponents), [](const auto& pComponent)
			{ return pComponent.get() == pComponent; }) == end(m_pComponents))
		{
			return;
		}

		pComponent->Destroy();
	}
}