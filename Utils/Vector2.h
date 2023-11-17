#pragma once

#include <vec2.hpp>

namespace leap
{
	class Vector2 final
	{
	public:
		// Vector2 is not constructable because it is NOT the object representation for a 2D vector, 
		//		use glm::vec2 for this
		// This class exists to give some helper functions to generate default 2D vector values
		Vector2() = delete;

		// Returns a glm::vec2 with value (0,0)
		static const glm::vec2& Zero() { return m_Zero; }

		// Returns a glm::vec2 with value (1,1)
		static const glm::vec2& One() { return m_One; }

		// Returns a glm::vec2 with value (1,0)
		static const glm::vec2& Right() { return m_Right; }

		// Returns a glm::vec2 with value (-1,0)
		static const glm::vec2& Left() { return m_Left; }

		// Returns a glm::vec2 with value (0,1)
		static const glm::vec2& Up() { return m_Up; }

		// Returns a glm::vec2 with value (0,-1)
		static const glm::vec2& Down() { return m_Down; }

	private:
		inline static glm::vec2 m_Zero{};
		inline static glm::vec2 m_One{ 1.0f, 1.0f };
		inline static glm::vec2 m_Right{ 1.0f, 0.0f };
		inline static glm::vec2 m_Left{ -1.0f, 0.0f };
		inline static glm::vec2 m_Up{ 0.0f, 1.0f };
		inline static glm::vec2 m_Down{ 0.0f, -1.0f };
	};
}