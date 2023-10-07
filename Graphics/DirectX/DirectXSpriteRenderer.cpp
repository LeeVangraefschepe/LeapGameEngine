#include "DirectXSpriteRenderer.h"

#include "../Data/Sprite.h"

#include <algorithm>

void leap::graphics::DirectXSpriteRenderer::AddSprite(Sprite* pSprite)
{
	m_pSprites.push_back(pSprite);
}

void leap::graphics::DirectXSpriteRenderer::RemoveSprite(Sprite* pSprite)
{
	m_pSprites.erase(std::remove(begin(m_pSprites), end(m_pSprites), pSprite));
}

void leap::graphics::DirectXSpriteRenderer::Draw() const
{
	for (Sprite* pSprite : m_pSprites)
	{
		DrawSprite(pSprite);
	}
}

void leap::graphics::DirectXSpriteRenderer::DrawSprite(Sprite* /*pSprite*/) const
{

}
