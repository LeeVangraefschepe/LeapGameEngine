#pragma once

#include "../../Component.h"
#include "ICanvasElement.h"

#include <Subject.h>

#include <vec4.hpp>

namespace leap
{
	class Image;
	class RectTransform;

	class Button final : public Component, public ICanvasElement
	{
	public:
		Button() = default;
		virtual ~Button() = default;

		Button(const Button& other) = delete;
		Button(Button&& other) = delete;
		Button& operator=(const Button& other) = delete;
		Button& operator=(Button&& other) = delete;

		TSubject<Button> OnClicked{};

		virtual glm::vec2 GetSize() override;
		virtual glm::vec3 GetPosition() override;
		virtual glm::vec2 GetPivot() override;

		void SetHoverColor(const glm::vec4& color);
		void SetClickColor(const glm::vec4& color);

		void SetImage(Image* pImage);

	protected:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		virtual void OnClickStart() override;
		virtual void OnClickEnd() override;
		virtual void OnHoverEnter() override;
		virtual void OnHoverExit() override;

	private:
		Image* m_pImage{};
		RectTransform* m_pTransform{};

		bool m_IsHovering{ false };
		bool m_IsClicked{ false };

		glm::vec4 m_OriginalColor{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 m_HoverColor{ 0.8f, 0.8f, 0.8f, 1.0f };
		glm::vec4 m_ClickColor{ 0.5f, 0.5f, 0.5f, 1.0f };
	};
}