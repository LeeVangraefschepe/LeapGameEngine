#pragma once

#include <vec3.hpp>

namespace leap
{
	class Vector3 final
	{
	public:
		// Vector3 is not constructable because it is NOT the object representation for a 3D vector, 
		//		use glm::vec3 for this
		// This class exists to give some helper functions to generate default 3D vector values
		Vector3() = delete;

		// Returns a glm::vec3 with value (0,0,0)
		static const glm::vec3& Zero() { return m_Zero; }

		// Returns a glm::vec3 with value (1,1,1)
		static const glm::vec3& One() { return m_One; }

		// Returns a glm::vec3 with value (1,0,0)
		static const glm::vec3& Right() 
		{ 
			return m_Right; 
		}

		// Returns a glm::vec3 with value (0,1,0)
		static const glm::vec3& Up() { return m_Up; }

		// Returns a glm::vec3 with value (0,0,1)
		static const glm::vec3& Forward() { return m_Forward; }

	private:
		inline static glm::vec3 m_Zero{};
		inline static glm::vec3 m_One{ 1.0f, 1.0f, 1.0f };
		inline static glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
		inline static glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		inline static glm::vec3 m_Forward{ 0.0f, 0.0f, 1.0f };
	};
}