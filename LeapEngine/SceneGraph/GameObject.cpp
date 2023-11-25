#include "GameObject.h"

#include "../Components/Transform/Transform.h"

#include "SceneManager.h"

unsigned int leap::GameObject::m_TransformComponentID = leap::ReflectionUtils::GenerateTypenameHash<Transform>();

leap::GameObject::GameObject(const char* name)
	: m_Name{ name }
{
	m_pTransform = AddComponent<Transform>();
}

void leap::GameObject::SetParent(GameObject* pParent)
{
	if (pParent == nullptr)
	{
		pParent = SceneManager::GetInstance().GetActiveScene()->GetRootObject();
	}

	GameObject* pPrevParent{ m_pParent };

	// You can't change the parent of the root gameobject
	if (pPrevParent == nullptr) return;

	// Don't do anything if you're assigning the same parent again
	if (pPrevParent == pParent) return;

	// Move the unique ptr of itself from the parent to this function
	const auto selfIt{ std::find_if(begin(pPrevParent->m_pChildren), end(pPrevParent->m_pChildren), [this](const auto& pChild) { return pChild.get() == this; }) };
	std::unique_ptr<GameObject> pSelf{ std::move(*selfIt) };
	
	// Set the new parent of this gameobject
	m_pParent = pParent;

	// Keep the world transform
	GetTransform()->KeepWorldTransform(pParent);
	
	// Add self to the new parent
	pParent->m_pChildren.emplace_back(std::move(pSelf));

	// Erase self from previous parent
	pPrevParent->m_pChildren.erase(selfIt);
}

leap::GameObject* leap::GameObject::CreateChild(const char* name)
{
	// Create a new gameobject
	auto pGameObject{ std::make_unique<GameObject>(name) };

	// Store the raw ptr
	GameObject* pRawGameObject{ pGameObject.get() };

	// Set the parent of the new gameobject and add it as a child
	pGameObject->m_pParent = this;
	m_pChildrenToAdd.emplace_back(std::move(pGameObject));

	// Return the raw ptr to the new gameobject
	return pRawGameObject;
}

leap::GameObject* leap::GameObject::GetChild(int index) const
{
	// Make sure the index is not out of range
	if (index < 0 || index >= static_cast<int>(m_pChildren.size()))
	{
		Debug::LogError("LeapEngine Error: GameObject::GetChild index out of bounds");
		return nullptr;
	}

	return m_pChildren[index].get();
}

void leap::GameObject::SetActive(bool isActive)
{
	// Set the next active state
	// The new active state will be handled at the end of the frame
	//		(e.g. call OnEnable/OnDisable)
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);

}

bool leap::GameObject::IsActive() const
{
	return IsActiveWorld();
}

void leap::GameObject::Destroy()
{
	m_StateFlags |= static_cast<unsigned char>(StateFlags::IsMarkedAsDead);
}

bool leap::GameObject::IsMarkedAsDead() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsMarkedAsDead);
}

void leap::GameObject::OnFrameStart()
{
	MoveNewObjectsAndComponents(); // Move components and children from the temp container to the normal container
	CallAwake(); // Call Awake on all new children and components
	ChangeActiveState(); // Update the local and world active states
	CallStart(); // Call the start when needed
	CallEnableAndDisable(); // Call OnEnable and OnDisable when needed
}

void leap::GameObject::MoveNewObjectsAndComponents()
{	
	// Move the components from the temp container to the default container
	for (auto& pComponent : m_ComponentsToAdd)
	{
		m_Components.emplace_back(std::move(pComponent));
	}

	// Move the children from the temp container to the default container
	for (auto& pChild : m_pChildrenToAdd)
	{
		m_pChildren.emplace_back(std::move(pChild));
	}

	// Clear the temp folders
	m_ComponentsToAdd.clear();
	m_pChildrenToAdd.clear();

	// Call MoveNewObjectsAndComponents on all children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->MoveNewObjectsAndComponents();
	}
}

void leap::GameObject::CallAwake() const
{
	// Call awake on the new components and children
	for (const auto& [pComponent, id] : m_Components)
	{
		if (!pComponent->IsInitialized())
		{
			pComponent->Initialize();
			pComponent->Awake();
		}
	}
	
	// Call awake on all children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->CallAwake();
	}
}

void leap::GameObject::ChangeActiveState()
{
	// Only update the active state if this gameobject is not the root gameobject
	//	and the active state has been changed
	if (m_pParent != nullptr && IsActiveLocalNextFrame() != IsActiveLocal())
	{
		// Set the new local state
		SetActiveLocal(IsActiveLocalNextFrame());

		// Update the world state of this object and its children
		SetWorldState(IsActiveLocal() && m_pParent->IsActive());
	}
	else if (m_pParent == nullptr)
	{
		// Ensure that the root gameobject is always enabled
		SetActiveLocal(true);
		SetActiveWorld(true);
	}

	// Update the active state of the components
	for (const auto& [pComponent, id] : m_Components) pComponent->ChangeActiveState();

	// Update the active state of the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->ChangeActiveState();
	}
}

void leap::GameObject::SetWorldState(bool isActive)
{
	// Update the world active state
	SetActiveWorld(isActive && IsActiveLocal());

	// Update the world state of every child
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->SetWorldState(isActive);
	}
}

void leap::GameObject::CallStart() const
{
	// Call Start on the components if needed
	for (const auto& [pComponent, id] : m_Components) pComponent->TryCallStart();

	// Call OnEnable or OnDisable on the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->CallStart();
	}
}

void leap::GameObject::CallEnableAndDisable() const
{
	// Call OnEnable or OnDisable on the components if needed
	for (const auto& [pComponent, id] : m_Components) pComponent->TryCallEnableAndDisable();

	// Call OnEnable or OnDisable on the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->CallEnableAndDisable();
	}
}

void leap::GameObject::OnFrameEnd()
{
	CheckDestroyFlag(); // Call OnDestroy on children and components
	UpdateCleanup(); // Remove children en components that are marked as dead
}

void leap::GameObject::CheckDestroyFlag() const
{
	// If this gameobject is the root gameobject, just delegate this method to the children
	if (m_pParent == nullptr)
	{
		// Call OnDestroy on every child if necessary
		for (const auto& pChild : m_pChildren) pChild->CheckDestroyFlag();
		return;
	}

	// Call OnDestroy on this gameobject and its children
	if (IsMarkedAsDead()) OnDestroy();
	else
	{
		// Call OnDestroy on components if needed
		for (const auto& [pComponent, id] : m_Components)
		{
			if (pComponent->IsMarkedAsDead()) pComponent->OnDestroy();
		}

		// Call OnDestroy on every child if necessary
		for (const auto& pChild : m_pChildren)
		{
			if (pChild) pChild->CheckDestroyFlag();
		}
	}
}

void leap::GameObject::UpdateCleanup()
{
	// Remove all marked components
	m_Components.erase(
		std::remove_if(
			begin(m_Components), end(m_Components), 
			[](const ComponentInfo& CInfo) { return CInfo.pComponent->IsMarkedAsDead(); }
		),
		end(m_Components));

	// Remove all children that are nullptr
	m_pChildren.erase(
		std::remove_if(
			begin(m_pChildren), end(m_pChildren),
			[](const auto& pChild) { return pChild.get() == nullptr; }
		),
		end(m_pChildren));

	// Remove all marked children
	m_pChildren.erase(
		std::remove_if(
			begin(m_pChildren), end(m_pChildren), 
			[](const auto& pChild) { return pChild->IsMarkedAsDead(); }
		),
		end(m_pChildren));

	// Cleanup every child
	for (const auto& pChild : m_pChildren) pChild->UpdateCleanup();
}

bool leap::GameObject::IsActiveLocalNextFrame() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);
}

bool leap::GameObject::IsActiveLocal() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveLocal);
}

void leap::GameObject::SetActiveLocal(bool isActive)
{
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveLocal);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveLocal);
}

bool leap::GameObject::IsActiveWorld() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveWorld);
}

void leap::GameObject::SetActiveWorld(bool isActive)
{
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveWorld);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveWorld);
}

leap::Transform* leap::GameObject::GetTransform() const
{
	return m_pTransform;
}

#pragma region ComponentMethods
// This method is called per gameobject, so it shouldn't delegate the method to its children
void leap::GameObject::OnEnable() const
{
	// Delegate OnEnable method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnEnable();
}

// This method is called per gameobject, so it shouldn't delegate the method to its children
void leap::GameObject::OnDisable() const
{
	// Delegate OnDisable method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnDisable();
}

void leap::GameObject::Update() const
{
	// Delegate Update method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->Update();

	// Delegate Update method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->Update();
	}
}

void leap::GameObject::FixedUpdate() const
{
	// Delegate FixedUpdate method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->FixedUpdate();

	// Delegate FixedUpdate method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->FixedUpdate();
	}
}

void leap::GameObject::LateUpdate() const
{
	// Delegate LateUpdate method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->LateUpdate();

	// Delegate LateUpdate method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->LateUpdate();
	}
}

void leap::GameObject::OnGUI() const
{
	// Delegate OnGUI method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnGUI();

	// Delegate OnGUI method to the children
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnGUI();
	}
}

void leap::GameObject::OnDestroy() const
{
	// Delegate OnDestroy method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnDestroy();

	// Delegate OnDestroy method to the children
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnDestroy();
	}
}

void leap::GameObject::OnCollisionEnter(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnCollisionEnter method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnCollisionEnter(pCollider, pOther);
}

void leap::GameObject::OnCollisionStay(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnCollisionStay method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnCollisionStay(pCollider, pOther);
}

void leap::GameObject::OnCollisionExit(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnCollisionExit method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnCollisionExit(pCollider, pOther);
}

void leap::GameObject::OnTriggerEnter(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnTriggerEnter method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnTriggerEnter(pCollider, pOther);
}

void leap::GameObject::OnTriggerStay(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnTriggerStay method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnTriggerStay(pCollider, pOther);
}

void leap::GameObject::OnTriggerExit(Collider* pCollider, Collider* pOther) const
{
	// Delegate OnTriggerExit method to the components
	for (const auto& [pComponent, id] : m_Components) pComponent->OnTriggerExit(pCollider, pOther);
}
#pragma endregion
