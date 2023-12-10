#pragma once

#include "../../Component.h"

#include <vec2.hpp>
#include <Vector2.h>
#include <vec3.hpp>

namespace leap
{
	class CanvasComponent;

	class RectTransform final : public Component
	{
	public:
		RectTransform() = default;
		virtual ~RectTransform() = default;

		RectTransform(const RectTransform& other) = delete;
		RectTransform(RectTransform&& other) = delete;
		RectTransform& operator=(const RectTransform& other) = delete;
		RectTransform& operator=(RectTransform&& other) = delete;

		void SetSize(const glm::vec2& size);
		void SetSize(float x, float y);
		const glm::vec2& GetSize();
		
		const glm::vec3& GetPosition();
		void SetReferencePosition(const glm::vec2& position);
		void SetReferencePosition(float x, float y);
		void SetLocalReferencePosition(const glm::vec2& position);
		void SetLocalReferencePosition(float x, float y);

		void SetDepth(float depth);

	protected:
		virtual void Awake() override;
		virtual void OnDestroy() override;

	private:
		void Resize();
		void UpdateSize();
		void UpdatePosition();
		void GetCanvas();
		void OnResolutionChanged(const glm::vec2& multiplier);

		bool m_IsSizeDirty{ true };
		bool m_IsPositionDirty{ true };
		bool m_IsPositionLocal{ true };

		CanvasComponent* m_pCanvas{};

		glm::vec2 m_ReferenceSize{ 100.0f, 100.0f };
		glm::vec2 m_Size{ 100.0f, 100.0f };

		glm::vec2 m_ReferencePosition{ Vector2::Zero() };

		float m_Depth{};
	};
}