#include "Camera.h"

leap::graphics::Camera::Camera(float aspectRatio, float fovAngle):
m_AspectRatio(aspectRatio)
{
	m_Fov = tanf((glm::radians(fovAngle) / 2));
	CalculateProjectionMatrix();
	CalculateViewMatrix();
}

void leap::graphics::Camera::SetRotation(const glm::quat& rotation)
{
	constexpr glm::vec3 forward{ 0,0,1 };
	m_Forward = forward * rotation;
	m_IsDirty = true;
}

const glm::mat4x4& leap::graphics::Camera::GetInverseViewMatrix()
{
	if (m_IsDirty) CalculateViewMatrix();
	return m_InverseViewMatrix;
}

const glm::mat4x4& leap::graphics::Camera::GetViewMatrix()
{
	if (m_IsDirty) CalculateViewMatrix();
	return m_ViewMatrix;
}

const glm::mat4x4& leap::graphics::Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void leap::graphics::Camera::CalculateViewMatrix()
{
	constexpr glm::vec3 UnitY{ 0,1,0 };
	const auto right = normalize(glm::cross(UnitY, m_Forward));
	const auto up = cross(m_Forward, right);

	m_InverseViewMatrix = glm::mat4x4
	{
		glm::vec4{ right, 0 },
		glm::vec4{ up, 0 },
		glm::vec4{ m_Forward, 0 },
		glm::vec4{ m_Position, 1 }
	};

	m_ViewMatrix = inverse(m_InverseViewMatrix);

	m_IsDirty = false;
}

void leap::graphics::Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::mat4x4
	{
		{ 1.f / (m_AspectRatio * m_Fov), 0, 0, 0 },
		{ 0, 1.f / m_Fov, 0, 0 },
		{ 0, 0, m_FarC / (m_FarC - m_NearC), 1 },
		{ 0, 0, -(m_FarC * m_NearC) / (m_FarC - m_NearC), 0 }
	};
}
