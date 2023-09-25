#pragma once
#include <vec4.hpp>
#include <vec3.hpp>
#pragma warning(disable: 4201)
#include <gtc/quaternion.hpp>
#pragma warning(default: 4201)

namespace leap::graphics
{
	class Camera
	{
	public:
		Camera(float aspectRatio, float fovAngle = 90.f);
		~Camera() = default;
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = delete;
		Camera& operator=(const Camera& other) = delete;
		Camera& operator=(Camera&& other) = delete;

		void SetColor(const glm::vec4& color) { m_Color = color; }
		void SetTransform(const glm::mat4x3& transform);

		const glm::vec4& GetColor() const { return m_Color; }
		const glm::mat4x4& GetInverseViewMatrix();
		const glm::mat4x4& GetViewMatrix();
		const glm::mat4x4& GetProjectionMatrix() const;

	private:
		glm::vec4 m_Color{ 0.39f, 0.59f, 0.93f, 1.f };
		glm::mat4x3 m_Transform{};
		glm::mat4x4 m_InverseViewMatrix{};
		glm::mat4x4 m_ViewMatrix{};
		bool m_IsDirty{};

		glm::mat4x4 m_ProjectionMatrix{};
		float m_AspectRatio;
		float m_Fov;
		float m_NearC = 0.1f;
		float m_FarC = 100.f;

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();
	};
}