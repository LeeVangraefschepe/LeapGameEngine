#include "RectTransform.h"

#include "../../../GameContext/GameContext.h"
#include "../../../GameContext/Window.h"

#include "../../../SceneGraph/GameObject.h"

#include "../../Transform/Transform.h"

#include "CanvasComponent.h"

#include "Debug.h"

void leap::RectTransform::SetSize(const glm::vec2& size)
{
	SetSize(size.x, size.y);
}

void leap::RectTransform::SetSize(float x, float y)
{
	m_ReferenceSize.x = x;
	m_ReferenceSize.y = y;
	m_IsSizeDirty = true;
}

const glm::vec2& leap::RectTransform::GetSize()
{
	if (m_IsSizeDirty) UpdateSize();

	return m_Size;
}

const glm::vec3& leap::RectTransform::GetPosition()
{
	if (m_IsPositionDirty) UpdatePosition();

	return GetTransform()->GetWorldPosition();
}

void leap::RectTransform::SetReferencePosition(const glm::vec2& position)
{
	SetReferencePosition(position.x, position.y);
}

void leap::RectTransform::SetReferencePosition(float x, float y)
{
	m_ReferencePosition.x = x;
	m_ReferencePosition.y = y;
	m_IsPositionLocal = false;
	m_IsPositionDirty = true;
}

void leap::RectTransform::SetLocalReferencePosition(const glm::vec2& position)
{
	SetLocalReferencePosition(position.x, position.y);
}

void leap::RectTransform::SetLocalReferencePosition(float x, float y)
{
	m_ReferencePosition.x = x;
	m_ReferencePosition.y = y;
	m_IsPositionLocal = true;
	m_IsPositionDirty = true;
}

void leap::RectTransform::SetDepth(float depth)
{
	m_Depth = depth;
	m_IsPositionDirty = true;
}

void leap::RectTransform::OnResolutionChanged(const glm::vec2&)
{
	Resize();
}

void leap::RectTransform::Awake()
{
	if (m_pCanvas == nullptr) GetCanvas();
	m_pCanvas->OnResolutionChanged.AddListener(this, &RectTransform::OnResolutionChanged);
}

void leap::RectTransform::OnDestroy()
{
	m_pCanvas->OnResolutionChanged.RemoveListener(this, &RectTransform::OnResolutionChanged);
}

void leap::RectTransform::Resize()
{
	m_IsSizeDirty = true;
	m_IsPositionDirty = true;
}

void leap::RectTransform::UpdateSize()
{
	m_IsSizeDirty = false;

	const glm::vec2& screenSize{ GameContext::GetInstance().GetWindow()->GetWindowSize() };
	const glm::vec2& reference{ m_pCanvas->GetReference() };
	const glm::vec2& canvasScale{ m_pCanvas->GetScale() };

	const glm::vec2& multiplier{ screenSize / reference };
	m_Size = multiplier * m_ReferenceSize * canvasScale;
}

void leap::RectTransform::UpdatePosition()
{
	m_IsPositionDirty = false;

	const glm::vec2& screenSize{ GameContext::GetInstance().GetWindow()->GetWindowSize() };
	const glm::vec2& reference{ m_pCanvas->GetReference() };
	const glm::vec2& canvasScale{ m_pCanvas->GetScale() };

	const glm::vec2& multiplier{ screenSize / reference };
	const glm::vec2& position{ multiplier * m_ReferencePosition * canvasScale };

	if (m_IsPositionLocal)
	{
		RectTransform* pParent{ GetGameObject()->GetParent()->GetComponent<RectTransform>() };
		if (pParent)
		{
			const glm::vec3& parentPos{ pParent->GetPosition() };
			GetTransform()->SetWorldPosition(parentPos.x + position.x, parentPos.y + position.y, m_Depth);
		}
		else
		{
			GetTransform()->SetWorldPosition(position.x, position.y, m_Depth);
		}
	}
	else
	{
		GetTransform()->SetWorldPosition(position.x, position.y, m_Depth);
	}
}

void leap::RectTransform::GetCanvas()
{
	GameObject* pParent{ GetGameObject()->GetParent() };
	m_pCanvas = pParent->GetComponent<CanvasComponent>();
	if (m_pCanvas == nullptr)
	{
		RectTransform* pParentRect{ pParent->GetComponent<RectTransform>() };
		if(pParentRect == nullptr)
		{
			Debug::LogError("LeapEngine Error : A RectTransform is a child of a gameobject that doesn't have a CanvasComponent or a RectTransform.");
			return;
		}

		m_pCanvas = pParentRect->m_pCanvas;
	}

	if (m_pCanvas == nullptr) Debug::LogError("LeapEngine Error : A RectTransform is a child of a gameobject that doesn't have a CanvasComponent or a RectTransform.");
}
