#pragma once

#include "../Component.h"

#include "Data/Sprite.h"

namespace leap
{
	namespace graphics
	{
		class ITexture;
	}

	class SpriteRendererComponent final : public Component
	{
	public:
		SpriteRendererComponent();
		virtual ~SpriteRendererComponent() = default;

		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		void SetTexture(graphics::ITexture* pTexture);
		void SetPivot(const glm::vec2& pivot);
		void SetPivot(float x, float y);
		void SetColor(const glm::vec4& color);
		void SetColor(float r, float g, float b);
		void SetColor(float r, float g, float b, float a);

		const graphics::ITexture* GetTexture() const;

	protected:
		virtual void OnDestroy() override;

	private:
		graphics::Sprite m_Sprite{};
	};
}