#include "Quaternion.h"

glm::quat leap::Quaternion::FromEuler(const glm::vec3& eulerAngles, bool degrees)
{
	return FromEuler(eulerAngles.x, eulerAngles.y, eulerAngles.z, degrees);
}

glm::quat leap::Quaternion::FromEuler(float x, float y, float z, bool degrees)
{
	const float radX{ degrees ? glm::radians(x) : x };
	const float radY{ degrees ? glm::radians(y) : y };
	const float radZ{ degrees ? glm::radians(z) : z };

	const float pitch{ radX / 2.0f };
	const float yaw{ radY / 2.0f };
	const float roll{ radZ / 2.0f };

	const float cp{ cosf(pitch) };
	const float sp{ sinf(pitch) };
	const float cy{ cosf(yaw) };
	const float sy{ sinf(yaw) };
	const float cr{ cosf(roll) };
	const float sr{ sinf(roll) };

	const glm::quat quaternion
	{
		cr * cp * cy + sr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		sr * cp * cy - cr * sp * sy
	};
	return quaternion;
}

glm::vec3 leap::Quaternion::ToEulerDegrees(const glm::quat& quaternion)
{
	return glm::degrees(ToEuler(quaternion));
}

glm::vec3 leap::Quaternion::ToEuler(const glm::quat& quaternion)
{
	const float sinr_cosp{ 2.0f * (quaternion.w * quaternion.y + quaternion.x * quaternion.z) };
	const float cosr_cosp{ 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.x * quaternion.x) };

	// pitch (y-axis rotation)
	const float sinp{ std::sqrtf(1 + 2.0f * (quaternion.w * quaternion.x - quaternion.y * quaternion.z)) };
	const float cosp{ std::sqrtf(1 - 2.0f * (quaternion.w * quaternion.x - quaternion.y * quaternion.z)) };

	// yaw (z-axis rotation)
	const float siny_cosp{ 2.0f * (quaternion.w * quaternion.z + quaternion.y * quaternion.x) };
	const float cosy_cosp{ 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z) };

	const glm::vec3 eulerAngles
	{
		2 * std::atan2f(sinp, cosp) - glm::pi<float>() / 2,
		std::atan2f(sinr_cosp, cosr_cosp),
		std::atan2f(siny_cosp, cosy_cosp)
	};
	return eulerAngles;
}
