#pragma once

#include <mat3x3.hpp>
#include <mat4x4.hpp>
#include <mat4x3.hpp>

namespace leap
{
	class Matrix final
	{
	public:
		// Matrix is not constructable because it is NOT the object representation for a matrix, 
		//		use glm::mat for this
		// This class exists to give some helper functions to generate default matrix values
		Matrix() = delete;

		static const glm::mat4x4& Identity4x4() { return m_Identity4x4; }
		static const glm::mat3x3& Identity3x3() { return m_Identity3x3; }
		static const glm::mat4x3& Identity4x3() { return m_Identity4x3; }

	private:
		inline static glm::mat4x4 m_Identity4x4
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		inline static glm::mat4x3 m_Identity4x3
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,
		};
		inline static glm::mat3x3 m_Identity3x3
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
	};
}