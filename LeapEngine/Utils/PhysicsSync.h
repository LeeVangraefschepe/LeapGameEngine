#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap
{
	class PhysicsSync final
	{
	public:
		static void SetTransform(void* pOwner, const glm::vec3& position, const glm::quat& rotation);
	};
}