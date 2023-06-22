#include "Component.h"

#include "../SceneGraph/GameObject.h"

void leap::Component::Destroy()
{
	m_IsMarkedDead = true;
}

void leap::Component::SetActive(bool isActive)
{
	// Set the next active state
	// The new active state will be handled at the end of the frame
	//		(e.g. call OnEnable/OnDisable)
	m_NextIsActiveLocal = isActive;
}

bool leap::Component::IsActive() const
{
	return m_IsActiveLocal && m_pOwner->IsActive();
}

void leap::Component::SetOwner(GameObject* pOwner)
{
	m_pOwner = pOwner;
}

void leap::Component::ChangeActiveState()
{
	// Cache the previous world state
	m_PrevIsActiveWorld = m_IsActiveWorld;

	if (m_NextIsActiveLocal != m_IsActiveLocal)
	{
		// Set the new local state
		m_IsActiveLocal = m_NextIsActiveLocal;
	}

	// Update the world state of this component
	m_IsActiveWorld = m_IsActiveLocal && m_pOwner->IsActive();
}

void leap::Component::TryCallStart()
{
	// Don't call start twice
	if (m_HasStarted) return;
	
	// Don't call start if the component is not active
	if (!m_IsActiveWorld) return;

	m_HasStarted = true;
	Start();
}

void leap::Component::TryCallEnableAndDisable()
{
	// Don't call a method if the worldstate hasn't changed
	if (m_PrevIsActiveWorld == m_IsActiveWorld) return;

	if (m_IsActiveWorld) OnEnable();
	else OnDisable();
}
