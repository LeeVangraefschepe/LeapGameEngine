#pragma once
#include <vec4.hpp>

namespace leap::graphics
{
	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = delete;
		Camera& operator=(const Camera& other) = delete;
		Camera& operator=(Camera&& other) = delete;

		const glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

	private:
		glm::vec4 m_Color{ 0.39f, 0.59f, 0.93f, 1.f };
	};
}