#include "Camera.h"

leap::graphics::Camera::Camera(float aspectRatio, float fovAngle):
m_AspectRatio(aspectRatio)
{
	m_Fov = tanf((glm::radians(fovAngle) / 2));
	CalculateProjectionMatrix();
	CalculateViewMatrix();
}

const glm::mat4x4& leap::graphics::Camera::GetInverseViewMatrix()
{
	if (m_IsDirty) CalculateViewMatrix();
	return m_InverseViewMatrix;
}

void leap::graphics::Camera::SetTransform(const glm::mat4x3& transform)
{
	m_Transform = transform;

	m_IsDirty = true;
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
	m_InverseViewMatrix = glm::mat4x4
	{
		glm::vec4{ m_Transform[0], 0 },
		glm::vec4{ m_Transform[1], 0 },
		glm::vec4{ m_Transform[2], 0 },
		glm::vec4{ m_Transform[3], 1 }
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
