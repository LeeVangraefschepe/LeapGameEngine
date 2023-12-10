#pragma once

#include "../../Component.h"

#include "Data/Sprite.h"

#include <vec2.hpp>

namespace leap
{
	class RectTransform;

	namespace graphics
	{
		class ITexture;
	}

	class Image final : public Component
	{
	public:
		Image();
		virtual ~Image() = default;

		Image(const Image& other) = delete;
		Image(Image&& other) = delete;
		Image& operator=(const Image& other) = delete;
		Image& operator=(Image&& other) = delete;

		void SetTexture(graphics::ITexture* pTexture);
		void SetPivot(const glm::vec2& pivot);
		void SetPivot(float x, float y);
		void SetColor(const glm::vec4& color);
		void SetColor(float r, float g, float b);
		void SetColor(float r, float g, float b, float a);
		void SetNativeSize();

		const graphics::ITexture* GetTexture() const;
		const glm::vec2& GetPivot() const;
		const glm::vec4& GetColor() const;

	private:
		graphics::Sprite m_Sprite{};
		RectTransform* m_pRect{};

		bool m_SetNative{};
	};
}