#include "Image.h"

#include "../../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

#include "../../Transform/Transform.h"
#include "RectTransform.h"

#include "../../../SceneGraph/GameObject.h"

#include "../../../GameContext/GameContext.h"
#include "../../../GameContext/Window.h"

leap::Image::Image()
{
	ServiceLocator::GetRenderer().AddSprite(&m_Sprite);
	m_Sprite.OnDraw = [this]() 
		{
			if (m_SetNative)
			{
				m_SetNative = false;

				if (m_pRect == nullptr) m_pRect = GetGameObject()->GetComponent<RectTransform>();
				m_pRect->SetSize(m_Sprite.pTexture->GetSize());
			}

			if (m_pRect == nullptr) m_pRect = GetGameObject()->GetComponent<RectTransform>();

			m_Sprite.vertex.position = m_pRect->GetPosition();
			m_Sprite.vertex.size = m_pRect->GetSize();
		};
}

void leap::Image::SetTexture(graphics::ITexture* pTexture)
{
	m_Sprite.pTexture = pTexture;
}

void leap::Image::SetPivot(const glm::vec2& pivot)
{
	m_Sprite.vertex.pivot = glm::clamp(pivot, 0.0f, 1.0f);
}

void leap::Image::SetPivot(float x, float y)
{
	SetPivot({ x,y });
}

void leap::Image::SetColor(const glm::vec4& color)
{
	m_Sprite.vertex.color = color;
}

void leap::Image::SetColor(float r, float g, float b)
{
	SetColor(r, g, b, 1.0f);
}

void leap::Image::SetColor(float r, float g, float b, float a)
{
	SetColor({ r,g,b,a });
}

void leap::Image::SetNativeSize()
{
	m_SetNative = true;
}

const leap::graphics::ITexture* leap::Image::GetTexture() const 
{
	return m_Sprite.pTexture;
}

const glm::vec2& leap::Image::GetPivot() const
{
	return m_Sprite.vertex.pivot;
}

const glm::vec4& leap::Image::GetColor() const
{
	return m_Sprite.vertex.color;
}
