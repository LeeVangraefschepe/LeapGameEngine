#pragma once

#include "../ShaderDelete.h"

#include <memory>

namespace leap::graphics
{
	struct Shader;

	namespace shaders
	{
		class PosNorm3D final
		{
		public:
			static std::unique_ptr<Shader, ShaderDelete> GetShader();
		};
	}
}