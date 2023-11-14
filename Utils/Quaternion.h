#pragma once

#include "vec3.hpp"
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap
{
	class Quaternion final
	{
	public:
		// Quaternion is not constructable because it is NOT the object representation for a quaternion, 
		//		use glm::quat for this
		// This class exists to give some helper functions to generate correct quaternions/eulerangles and default quaternions
		Quaternion() = delete;

		static glm::quat FromEuler(const glm::vec3& eulerAngles, bool degrees = true);
		static glm::quat FromEuler(float x, float y, float z, bool degrees = true);
		static glm::vec3 ToEuler(const glm::quat& quaternion);
		static glm::vec3 ToEulerDegrees(const glm::quat& quaternion);		

		static const glm::quat& Identity() { return m_Identity; }

	private:
		inline static glm::quat m_Identity{ 1.0f, 0.0f, 0.0f, 0.0f };
	};
}