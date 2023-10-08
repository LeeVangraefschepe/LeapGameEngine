#pragma once

#include "../../Component.h"

#include <Observer.h>

#include <vec2.hpp>

namespace leap
{
	class CanvasComponent final : public Component, public Observer<glm::ivec2>
	{
	public:
		enum class MatchMode
		{
			MatchWidth,
			MatchHeight
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

	protected:
		virtual void Awake() override;
		virtual void OnDestroy() override;

	private:
		virtual void Notify(const glm::ivec2& size) override;

		void UpdateResolution(const glm::ivec2& size);

		MatchMode m_MatchMode{ MatchMode::MatchHeight };
		glm::ivec2 m_ReferenceResolution{ 1920, 1080 };
	};
}