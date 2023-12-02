#pragma once

#include <vector>
#include <memory>

#include <vec2.hpp>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace leap::graphics
{
	class DirectXEngine;
	struct Sprite;
	class DirectXMaterial;

	class DirectXSpriteRenderer final
	{
	public:
		DirectXSpriteRenderer() = default;
		~DirectXSpriteRenderer() = default;
		DirectXSpriteRenderer(const DirectXSpriteRenderer& other) = delete;
		DirectXSpriteRenderer(DirectXSpriteRenderer&& other) = delete;
		DirectXSpriteRenderer& operator=(const DirectXSpriteRenderer& other) = delete;
		DirectXSpriteRenderer& operator=(DirectXSpriteRenderer&& other) = delete;

		void Create(DirectXEngine* pEngine, const glm::vec2& screenSize);

		void AddSprite(Sprite* pSprite);
		void RemoveSprite(Sprite* pSprite);

		void Draw();

	private:
		void DrawSprite(Sprite* pSprite) const;

		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};
		std::unique_ptr<DirectXMaterial> m_pMaterial{};

		std::vector<Sprite*> m_pSprites{};
	};
}