#pragma once

namespace leap::graphics
{
	struct Shader;

	struct ShaderDelete final
	{
		void operator()(Shader* pShader) const;
	};
}