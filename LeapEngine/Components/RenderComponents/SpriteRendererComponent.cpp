#include "SpriteRendererComponent.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

leap::SpriteRendererComponent::SpriteRendererComponent()
{
	ServiceLocator::GetRenderer().AddSprite(&m_Sprite);
}

void leap::SpriteRendererComponent::SetTexture(graphics::ITexture* pTexture)
{
	m_Sprite.pTexture = pTexture;
}

void leap::SpriteRendererComponent::SetPivot(const glm::vec2& pivot)
{
	m_Sprite.vertex.pivot = pivot;
}

void leap::SpriteRendererComponent::SetColor(const glm::vec4& color)
{
	m_Sprite.vertex.color = color;
}

void leap::SpriteRendererComponent::OnDestroy()
{
	ServiceLocator::GetRenderer().RemoveSprite(&m_Sprite);
}
