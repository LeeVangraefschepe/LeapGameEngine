#pragma once

#include "../Components/Component.h"
#include "Debug.h"
#include "ReflectionUtils.h"

#include <string>
#include <memory>
#include <vector>

namespace leap
{
	class Transform;
	class Scene;
	class Collider;
	class PhysicsSync;

	class GameObject final
	{
		struct ComponentInfo
		{
			std::unique_ptr<Component> pComponent;
			unsigned int id;
		};

		static unsigned int m_TransformComponentID;

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
		T* GetComponentInParent() const;
		template <class T>
		void RemoveComponent();
		template <class T>
		void RemoveComponent(T* pComponent);

		Transform* GetTransform() const;
	private:
		friend Scene;
		friend PhysicsSync;

		void OnEnable() const;
		void OnDisable() const;
		void Update() const;
		void FixedUpdate() const;
		void LateUpdate() const;
		void OnGUI() const;
		void OnDestroy() const;
		void OnCollisionEnter(Collider* pCollider, Collider* pOther) const;
		void OnCollisionStay(Collider* pCollider, Collider* pOther) const;
		void OnCollisionExit(Collider* pCollider, Collider* pOther) const;
		void OnTriggerEnter(Collider* pCollider, Collider* pOther) const;
		void OnTriggerStay(Collider* pCollider, Collider* pOther) const;
		void OnTriggerExit(Collider* pCollider, Collider* pOther) const;

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

		std::vector<ComponentInfo> m_ComponentsToAdd{};
		std::vector<ComponentInfo> m_Components{};
	};

	template<class T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		constexpr uint32_t componentID{ ReflectionUtils::GenerateTypenameHash<T>() };
		if (componentID == m_TransformComponentID && HasComponent<T>())
		{
			Debug::LogError("LeapEngine Error: GameObject::AddComponent() > Can't add multiple Transforms");
			return nullptr;
		}

		ComponentInfo& CInfo{ m_ComponentsToAdd.emplace_back(std::make_unique<T>(), componentID) };

		CInfo.pComponent->SetOwner(this);

		return static_cast<T*>(CInfo.pComponent.get());
	}

	template<class T>
	inline bool GameObject::HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}

	template<class T>
	inline T* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		constexpr uint32_t componentID{ ReflectionUtils::GenerateTypenameHash<T>() };

		const auto iterator
		{
			std::find_if(m_Components.begin(), m_Components.end(),
			[componentID](const ComponentInfo& CInfo)
			{
				return componentID == CInfo.id;
			})
		};

		return iterator != m_Components.end() ? static_cast<T*>(iterator->pComponent.get()) : nullptr;
	}

	template<class T>
	inline std::vector<T*> GameObject::GetComponents() const
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		std::vector<T*> pComponents{};
		constexpr uint32_t componentID{ ReflectionUtils::GenerateTypenameHash<T>() };

		for (const ComponentInfo& CInfo : m_Components)
		{
			if (componentID == CInfo.id)
			{
				pComponents.push_back(static_cast<T*>(CInfo.pComponent.get()));
			}
		}

		return pComponents;
	}

	template<class T>
	inline T* GameObject::GetComponentInParent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		GameObject* pParent{ GetParent() };
		if (pParent == nullptr) return nullptr;

		T* pComponent{ pParent->GetComponent<T>() };
		if (pComponent != nullptr) return pComponent;

		return pParent->GetComponentInParent<T>();
	}

	template<class T>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		constexpr uint32_t componentID{ ReflectionUtils::GenerateTypenameHash<T>() };
		if (componentID == m_TransformComponentID)
		{
			Debug::LogError("LeapEngine Error: GameObject::RemoveComponent() > Cannot manually remove Transform");
			return;
		}

		GetComponent<T>()->Destroy();
	}

	template<class T>
	inline void GameObject::RemoveComponent(T* pComponent)
	{
		static_assert(std::is_base_of_v<Component, T>, "T needs to be derived from the Component class");

		constexpr uint32_t componentID{ ReflectionUtils::GenerateTypenameHash<T>() };
		if (componentID == m_TransformComponentID)
		{
			Debug::LogError("LeapEngine Error: GameObject::RemoveComponent() > Cannot manually remove Transform");
			return;
		}

		// Don't try to remove a component that is not on this gameobject
		if (std::find_if(begin(m_Components), end(m_Components), [](const ComponentInfo& CInfo)
			{ return CInfo.pComponent.get() == pComponent; }) == end(m_Components))
		{
			return;
		}

		pComponent->Destroy();
	}
}