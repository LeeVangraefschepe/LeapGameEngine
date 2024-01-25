#pragma once

#include "../ShaderDelete.h"

#include <memory>

namespace leap::graphics
{
	struct Shader;

	namespace shaders
	{
		class Billboard final
		{
		public:
			static std::unique_ptr<Shader, ShaderDelete> GetShader();
		};
	}
}