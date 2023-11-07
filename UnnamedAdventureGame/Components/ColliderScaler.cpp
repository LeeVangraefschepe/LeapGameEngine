#include "ColliderScaler.h"

#include <vec3.hpp>
#include <Components/Transform/Transform.h>

#include <GameContext/Timer.h>
#include <GameContext/GameContext.h>

void unag::ColliderScaler::Awake()
{
}

void unag::ColliderScaler::Update()
{
	m_Scale += m_Direction * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime();
	if (m_Scale > m_MaxScale) m_Direction = -1.0f;
	else if (m_Scale < m_MinScale) m_Direction = 1.0f;

	GetTransform()->SetWorldScale(m_Scale);
}
