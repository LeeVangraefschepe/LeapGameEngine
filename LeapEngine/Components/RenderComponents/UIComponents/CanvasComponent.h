#pragma once

#include "../../Component.h"

#include <Subject.h>

#include <vec2.hpp>
#include <Vector2.h>

namespace leap
{
	class CanvasComponent final : public Component
	{
	public:
		enum class MatchMode
		{
			MatchWidth,
			MatchHeight,
			Stretch
		};

		CanvasComponent() = default;
		virtual ~CanvasComponent() = default;

		CanvasComponent(const CanvasComponent& other) = delete;
		CanvasComponent(CanvasComponent&& other) = delete;
		CanvasComponent& operator=(const CanvasComponent& other) = delete;
		CanvasComponent& operator=(CanvasComponent&& other) = delete;

		void SetReferenceResolution(const glm::ivec2& resolution);
		void SetMatchMode(MatchMode matchMode);

		const glm::ivec2& GetReference() const { return m_ReferenceResolution; }
		const glm::vec2& GetScale() const { return m_CurrentScale; }

		TSubject<glm::vec2> OnResolutionChanged{};

	protected:
		virtual void Awake() override;
		virtual void OnDestroy() override;

	private:
		void UpdateResolution(const glm::ivec2& size);

		MatchMode m_MatchMode{ MatchMode::MatchHeight };
		glm::ivec2 m_ReferenceResolution{ 1920, 1080 };
		glm::vec2 m_CurrentScale{ Vector2::One() };
	};
}