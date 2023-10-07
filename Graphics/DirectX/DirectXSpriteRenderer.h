#pragma once

#include <vector>

namespace leap::graphics
{
	struct Sprite;

	class DirectXSpriteRenderer final
	{
	public:
		DirectXSpriteRenderer() = default;
		~DirectXSpriteRenderer() = default;
		DirectXSpriteRenderer(const DirectXSpriteRenderer& other) = delete;
		DirectXSpriteRenderer(DirectXSpriteRenderer&& other) = delete;
		DirectXSpriteRenderer& operator=(const DirectXSpriteRenderer& other) = delete;
		DirectXSpriteRenderer& operator=(DirectXSpriteRenderer&& other) = delete;

		void AddSprite(Sprite* pSprite);
		void RemoveSprite(Sprite* pSprite);

		void Draw() const;

	private:
		void DrawSprite(Sprite* pSprite) const;

		std::vector<Sprite*> m_pSprites{};
	};
}