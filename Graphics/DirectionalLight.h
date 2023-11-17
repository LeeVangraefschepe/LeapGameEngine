#pragma once

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <Vector3.h>

namespace leap::graphics
{
	class DirectionalLight final
	{
	public:
		DirectionalLight();
		~DirectionalLight() = default;

		void SetShadowMapData(float aspectRatio, float size, float nearPlane, float farPlane);
		void SetTransform(const glm::mat4x3& transform);

		const glm::vec2& GetShadowMapSize() const { return m_Size; }
		const glm::vec3& GetDirection() const { return m_Forward; }
		const glm::mat4x4& GetViewProjection() const { return m_ViewProj; }
	private:
		glm::vec2 m_Size{ 1280.0f / 720.0f * 10.0f, 10.0f };
		glm::vec3 m_Forward{ Vector3::Forward() };
		glm::mat4x4 m_ViewProj{};
		float m_NearPlane{ 0.1f };
		float m_FarPlane{ 200.0f };
	};
}