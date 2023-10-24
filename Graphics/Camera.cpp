#include "Camera.h"

leap::graphics::Camera::Camera(float width, float height, float fovAngle)
	: m_Size{ width, height }
	, m_Fov{ glm::radians(fovAngle) }
{
	CalculateProjectionMatrix();
	CalculateViewMatrix();
}

const glm::mat4x4& leap::graphics::Camera::GetInverseViewMatrix()
{
	if (m_IsDirty) CalculateViewMatrix();
	return m_InverseViewMatrix;
}

void leap::graphics::Camera::SetAspectRatio(const glm::ivec2& size)
{
	m_Size = size;
	CalculateProjectionMatrix();
}

void leap::graphics::Camera::SetTransform(const glm::mat4x3& transform)
{
	m_Transform = transform;
	m_IsDirty = true;
}

void leap::graphics::Camera::SetFarPlane(float farPlane)
{
	m_FarC = farPlane;
	CalculateProjectionMatrix();
}

void leap::graphics::Camera::SetNearPlane(float nearPlane)
{
	m_NearC = nearPlane;
	CalculateProjectionMatrix();
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
	m_ProjectionMatrix = glm::perspectiveFovLH(m_Fov, m_Size.x, m_Size.y, m_NearC, m_FarC);
}
