#include "Component.h"

#include "../SceneGraph/GameObject.h"

leap::Transform* leap::Component::GetTransform() const
{
	return m_pOwner->GetTransform();
}

void leap::Component::Destroy()
{
	m_StateFlags |= static_cast<unsigned char>(StateFlags::IsMarkedAsDead);
}

bool leap::Component::IsMarkedAsDead() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsMarkedAsDead);
}

void leap::Component::SetActive(bool isActive)
{
	// Set the next active state
	// The new active state will be handled at the end of the frame
	//		(e.g. call OnEnable/OnDisable)
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);
}

bool leap::Component::IsActive() const
{
	return IsActiveWorld();
}

void leap::Component::SetOwner(GameObject* pOwner)
{
	m_pOwner = pOwner;
}

void leap::Component::ChangeActiveState()
{
	// Cache the previous world state
	SetPreviousActiveWorld(IsActiveWorld());

	// Retrieve the local states
	const bool isActiveLocalNextFrame{ IsActiveLocalNextFrame() };

	// Only change the local state if necessary
	if (isActiveLocalNextFrame != IsActiveLocal())
	{
		// Set the new local state
		SetActiveLocal(isActiveLocalNextFrame);
	}

	// Update the world state of this component
	SetActiveWorld(IsActiveLocal() && m_pOwner->IsActive());
}

void leap::Component::TryCallStart()
{
	// Don't call start twice
	if (HasStarted()) return;
	
	// Don't call start if the component is not active
	if (!IsActiveWorld()) return;

	// Active the started bit flag
	m_StateFlags |= static_cast<unsigned char>(StateFlags::HasStarted);

	Start();
}

void leap::Component::TryCallEnableAndDisable()
{
	const bool isActiveWorld{ IsActiveWorld() };

	// Don't call a method if the worldstate hasn't changed
	if (WasActiveWorldPreviousFrame() == isActiveWorld) return;

	// Call the appropriate method
	if (isActiveWorld) OnEnable();
	else OnDisable();
}

bool leap::Component::IsActiveLocalNextFrame() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame);
}

bool leap::Component::IsActiveLocal() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveLocal);
}

void leap::Component::SetActiveLocal(bool isActive)
{
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveLocal);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveLocal);
}

bool leap::Component::IsActiveWorld() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsActiveWorld);
}

void leap::Component::SetActiveWorld(bool isActive)
{
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::IsActiveWorld);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::IsActiveWorld);
}

bool leap::Component::WasActiveWorldPreviousFrame() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::WasActiveWorldPreviousFrame);
}

void leap::Component::SetPreviousActiveWorld(bool isActive)
{
	if (isActive)
		m_StateFlags |= static_cast<unsigned char>(StateFlags::WasActiveWorldPreviousFrame);
	else
		m_StateFlags &= ~static_cast<unsigned char>(StateFlags::WasActiveWorldPreviousFrame);
}

bool leap::Component::HasStarted() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::HasStarted);
}

bool leap::Component::IsInitialized() const
{
	return m_StateFlags & static_cast<unsigned char>(StateFlags::IsInitialized);
}

void leap::Component::Initialize()
{
	m_StateFlags |= static_cast<unsigned char>(StateFlags::IsInitialized);
}
