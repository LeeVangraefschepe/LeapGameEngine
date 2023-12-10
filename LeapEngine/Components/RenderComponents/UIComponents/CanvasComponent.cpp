#include "CanvasComponent.h"

#include "../../../GameContext/GameContext.h"
#include "../../../GameContext/Window.h"

#include "../../Transform/Transform.h"

void leap::CanvasComponent::SetReferenceResolution(const glm::ivec2& resolution)
{
	m_ReferenceResolution = resolution;
	UpdateResolution(GameContext::GetInstance().GetWindow()->GetWindowSize());
}

void leap::CanvasComponent::SetMatchMode(MatchMode matchMode)
{
	m_MatchMode = matchMode;
	UpdateResolution(GameContext::GetInstance().GetWindow()->GetWindowSize());
}

void leap::CanvasComponent::Awake()
{
	GameContext::GetInstance().GetWindow()->AddListener(this, &CanvasComponent::UpdateResolution);
}

void leap::CanvasComponent::OnDestroy()
{
	GameContext::GetInstance().GetWindow()->RemoveListener(this, &CanvasComponent::UpdateResolution);
}

void leap::CanvasComponent::UpdateResolution(const glm::ivec2& size)
{
	switch (m_MatchMode)
	{
	case MatchMode::MatchWidth:
	{
		const float width{ static_cast<float>(size.x) };
		const float heightMultiplier{ static_cast<float>(m_ReferenceResolution.y) / m_ReferenceResolution.x };
		const float height{ heightMultiplier * width };

		m_CurrentScale.x = 1.0f;
		m_CurrentScale.y = height / size.y;

		OnResolutionChanged.Notify(m_CurrentScale);
		break;
	}
	case MatchMode::MatchHeight:
	{
		const float height{ static_cast<float>(size.y) };
		const float widthMultiplier{ static_cast<float>(m_ReferenceResolution.x) / m_ReferenceResolution.y };
		const float width{ widthMultiplier * height };

		m_CurrentScale.x = width / size.x;
		m_CurrentScale.y = 1.0f;

		OnResolutionChanged.Notify(m_CurrentScale);
		break;
	}
	case MatchMode::Stretch:
	{
		m_CurrentScale.x = 1.0f;
		m_CurrentScale.y = 1.0f;

		OnResolutionChanged.Notify(m_CurrentScale);
		break;
	}
	}
}
