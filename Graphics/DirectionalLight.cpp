#include "DirectionalLight.h"

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>

leap::graphics::DirectionalLight::DirectionalLight()
{
	SetTransform(
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f }
		}
	);
}

void leap::graphics::DirectionalLight::SetShadowMapData(float aspectRatio, float size, float nearPlane, float farPlane)
{
	m_Size = { aspectRatio * size, size };
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
}

void leap::graphics::DirectionalLight::SetTransform(const glm::mat4x3& transform)
{
	auto projMatrix = glm::orthoLH_ZO(-m_Size.x, m_Size.x, -m_Size.y, m_Size.y, m_NearPlane, m_FarPlane);

	auto inverseViewMatrix = glm::mat4x4
	{
		glm::vec4{ transform[0], 0 },
		glm::vec4{ transform[1], 0 },
		glm::vec4{ transform[2], 0 },
		glm::vec4{ transform[3], 1 }
	};

	m_ViewProj = projMatrix * glm::inverse(inverseViewMatrix);
}
