#pragma once

#include <ShaderDelete.h>

#include <memory>

namespace leap
{
	namespace graphics
	{
		struct Shader;
	}

	template <typename T>
	class Shader final
	{
	public:
		std::unique_ptr<graphics::Shader, graphics::ShaderDelete> GetShader() const
		{
			return T::GetShader();
		}
	};
}