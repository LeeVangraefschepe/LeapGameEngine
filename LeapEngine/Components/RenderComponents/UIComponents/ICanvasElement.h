#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

namespace leap
{
	class CanvasActions;

	class ICanvasElement
	{
	public:
		virtual ~ICanvasElement() = default;

		virtual glm::vec2 GetSize() = 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::vec2 GetPivot() = 0;

	protected:
		virtual void OnClickStart() {};
		virtual void OnClick() {};
		virtual void OnClickEnd() {};
		virtual void OnHoverEnter() {};
		virtual void OnHover() {};
		virtual void OnHoverExit() {};

		friend CanvasActions;
	};
}