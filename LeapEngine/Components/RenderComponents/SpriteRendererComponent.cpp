#include "SpriteRendererComponent.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

#include "../Transform/Transform.h"

leap::SpriteRendererComponent::SpriteRendererComponent()
{
	ServiceLocator::GetRenderer().AddSprite(&m_Sprite);
	m_Sprite.OnDraw = [this]() 
		{
			m_Sprite.vertex.position = GetTransform()->GetWorldPosition();
			m_Sprite.vertex.size = GetTransform()->GetWorldScale();
		};
}

void leap::SpriteRendererComponent::SetTexture(graphics::ITexture* pTexture)
{
	m_Sprite.pTexture = pTexture;
}

void leap::SpriteRendererComponent::SetPivot(const glm::vec2& pivot)
{
	m_Sprite.vertex.pivot = glm::clamp(pivot, 0.0f, 1.0f);
}

void leap::SpriteRendererComponent::SetPivot(float x, float y)
{
	SetPivot({ x,y });
}

void leap::SpriteRendererComponent::SetColor(const glm::vec4& color)
{
	m_Sprite.vertex.color = color;
}

void leap::SpriteRendererComponent::SetColor(float r, float g, float b)
{
	SetColor(r, g, b, 1.0f);
}

void leap::SpriteRendererComponent::SetColor(float r, float g, float b, float a)
{
	SetColor({ r,g,b,a });
}

void leap::SpriteRendererComponent::OnDestroy()
{
	ServiceLocator::GetRenderer().RemoveSprite(&m_Sprite);
}
