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
	GameContext::GetInstance().GetWindow()->AddListener(this);
}

void leap::CanvasComponent::OnDestroy()
{
	GameContext::GetInstance().GetWindow()->RemoveListener(this);
}

void leap::CanvasComponent::Notify(const glm::ivec2& size)
{
	UpdateResolution(size);
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

		GetTransform()->SetWorldScale(width, height, 1.0f);
		break;
	}
	case MatchMode::MatchHeight:
	{
		const float height{ static_cast<float>(size.y) };
		const float widthMultiplier{ static_cast<float>(m_ReferenceResolution.x) / m_ReferenceResolution.y };
		const float width{ widthMultiplier * height };

		GetTransform()->SetWorldScale(width, height, 1.0f);
		break;
	}
	}
}
