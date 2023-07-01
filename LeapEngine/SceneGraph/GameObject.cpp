#include "GameObject.h"

#include <stdexcept>

#include "SceneManager.h"

leap::GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
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
	
	// Add self to the new parent
	pParent->m_pChildrenToAdd.emplace_back(std::move(pSelf));

	// Set the new parent of this gameobject
	m_pParent = pParent;

	// TODO: Set transform data
}

leap::GameObject* leap::GameObject::CreateChild(const std::string& name)
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
		throw std::runtime_error("GameObject Error: GetChild index out of bounds");
	}

	return m_pChildren[index].get();
}

void leap::GameObject::SetActive(bool isActive)
{
	// Set the next active state
	// The new active state will be handled at the end of the frame
	//		(e.g. call OnEnable/OnDisable)
	m_NextIsActiveLocal = isActive;
}

void leap::GameObject::Destroy()
{
	m_IsMarkedDead = true;
}

void leap::GameObject::OnFrameStart()
{
	MoveNewObjectsAndComponents(); // Move components and children from the temp container to the normal container
	CallAwake(); // Call Awake on all new children and components
	OnFrameStartCleanup(); // Clear all the temp containers
	ChangeActiveState(); // Update the local and world active states
	CallStart(); // Call the start when needed
	CallEnableAndDisable(); // Call OnEnable and OnDisable when needed
}

void leap::GameObject::MoveNewObjectsAndComponents()
{	
	// Store the raw new components and move the components from the temp container to the default container
	for (auto& pComponent : m_pComponentsToAdd)
	{
		m_pNewestComponents.emplace_back(pComponent.get());
		m_pComponents.emplace_back(std::move(pComponent));
	}

	// Store the raw new children and move the children from the temp container to the default container
	for (auto& pChild : m_pChildrenToAdd)
	{
		m_pNewestChildren.emplace_back(pChild.get());
		m_pChildren.emplace_back(std::move(pChild));
	}

	// Clear the temp folders
	m_pComponentsToAdd.clear();
	m_pChildrenToAdd.clear();

	// Call MoveNewObjectsAndComponents on all children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->MoveNewObjectsAndComponents();
	}
}

void leap::GameObject::CallAwake() const
{
	// Call awake on all the new components and children
	for (auto pComponent : m_pNewestComponents) pComponent->Awake();

	// Call awake on all children
	for (const auto& pChild : m_pNewestChildren)
	{
		if (pChild) pChild->CallAwake();
	}
}

void leap::GameObject::OnFrameStartCleanup()
{
	// Clear all the new object containers
	m_pNewestChildren.clear();
	m_pNewestComponents.clear();

	// Clear all new containers of the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->OnFrameStartCleanup();
	}
}

void leap::GameObject::ChangeActiveState()
{
	// Only update the active state if this gameobject is not the root gameobject
	//	and the active state has been changed
	if (m_pParent != nullptr && m_NextIsActiveLocal != m_IsActiveLocal)
	{
		// Set the new local state
		m_IsActiveLocal = m_NextIsActiveLocal;

		// Update the world state of this object and its children
		SetWorldState(m_IsActiveLocal && m_pParent->IsActive());
	}
	else if (m_pParent == nullptr)
	{
		// Ensure that the root gameobject is always enabled
		m_IsActiveLocal = true;
		m_IsActiveWorld = true;
	}

	// Update the active state of the components
	for (const auto& pComponent : m_pComponents) pComponent->ChangeActiveState();

	// Update the active state of the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->ChangeActiveState();
	}
}

void leap::GameObject::SetWorldState(bool isActive)
{
	// Update the world active state
	m_IsActiveWorld = isActive && m_IsActiveLocal;

	// Update the world state of every child
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->SetWorldState(isActive);
	}
}

void leap::GameObject::CallStart() const
{
	// Call Start on the components if needed
	for (const auto& pComponent : m_pComponents) pComponent->TryCallStart();

	// Call OnEnable or OnDisable on the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->CallStart();
	}
}

void leap::GameObject::CallEnableAndDisable() const
{
	// Call OnEnable or OnDisable on the components if needed
	for (const auto& pComponent : m_pComponents) pComponent->TryCallEnableAndDisable();

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
	if (m_IsMarkedDead) OnDestroy();
	else
	{
		// Call OnDestroy on components if needed
		for (const auto& pComponent : m_pComponents)
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
	m_pComponents.erase(
		std::remove_if(
			begin(m_pComponents), end(m_pComponents), 
			[](const auto& pComponent) { return pComponent->IsMarkedAsDead(); }
		),
		end(m_pComponents));

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

#pragma region ComponentMethods
// This method is called per gameobject, so it shouldn't delegate the method to its children
void leap::GameObject::OnEnable() const
{
	// Delegate OnEnable method to the components
	for (const auto& pComponent : m_pComponents) pComponent->OnEnable();
}

// This method is called per gameobject, so it shouldn't delegate the method to its children
void leap::GameObject::OnDisable() const
{
	// Delegate OnDisable method to the components
	for (const auto& pComponent : m_pComponents) pComponent->OnDisable();
}

void leap::GameObject::Update() const
{
	// Delegate Update method to the components
	for (const auto& pComponent : m_pComponents) pComponent->Update();

	// Delegate Update method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->Update();
	}
}

void leap::GameObject::FixedUpdate() const
{
	// Delegate FixedUpdate method to the components
	for (const auto& pComponent : m_pComponents) pComponent->FixedUpdate();

	// Delegate FixedUpdate method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->FixedUpdate();
	}
}

void leap::GameObject::LateUpdate() const
{
	// Delegate LateUpdate method to the components
	for (const auto& pComponent : m_pComponents) pComponent->LateUpdate();

	// Delegate LateUpdate method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->LateUpdate();
	}
}

void leap::GameObject::Render() const
{
	// Delegate Render method to the components
	for (const auto& pComponent : m_pComponents) pComponent->Render();

	// Delegate Render method to the children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild) pChild->Render();
	}
}

void leap::GameObject::OnGUI() const
{
	// Delegate OnGUI method to the components
	for (const auto& pComponent : m_pComponents) pComponent->OnGUI();

	// Delegate OnGUI method to the children
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnGUI();
	}
}

void leap::GameObject::OnDestroy() const
{
	// Delegate OnDestroy method to the components
	for (const auto& pComponent : m_pComponents) pComponent->OnDestroy();

	// Delegate OnDestroy method to the children
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnDestroy();
	}
}
#pragma endregion
