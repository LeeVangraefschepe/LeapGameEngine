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
		Camera(float width, float height, float fovAngle = 90.f);
		~Camera() = default;
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = delete;
		Camera& operator=(const Camera& other) = delete;
		Camera& operator=(Camera&& other) = delete;

		void SetAspectRatio(const glm::ivec2& size);
		void SetColor(const glm::vec4& color) { m_Color = color; }
		void SetTransform(const glm::mat4x3& transform);
		void SetFarPlane(float farPlane);
		void SetNearPlane(float nearPlane);

		const glm::vec4& GetColor() const { return m_Color; }
		const glm::mat4x4& GetInverseViewMatrix();
		const glm::mat4x4& GetViewMatrix();
		const glm::mat4x4& GetProjectionMatrix() const;
		float GetFarPlane() const { return m_FarC; }
		float GetNearPlane() const { return m_NearC; }

	private:
		glm::vec4 m_Color{ 0.39f, 0.59f, 0.93f, 1.f };
		glm::mat4x3 m_Transform{};
		glm::mat4x4 m_InverseViewMatrix{};
		glm::mat4x4 m_ViewMatrix{};
		bool m_IsDirty{};

		glm::mat4x4 m_ProjectionMatrix{};
		glm::vec2 m_Size{ 1280.0f, 720.0f };
		float m_Fov;
		float m_NearC = 0.1f;
		float m_FarC = 100.f;

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();
	};
}