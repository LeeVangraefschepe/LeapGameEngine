#pragma once

#include "../ShaderDelete.h"

#include <d3dx11effect.h>

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace leap::graphics
{
	struct Shader;

	struct DirectXShader final
	{
		std::string path{};
		std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> vertexDataFunction{};
	};

	class DirectXShaderReader final
	{
	public:
		static DirectXShader GetShaderData(std::unique_ptr<Shader, ShaderDelete> pShader);
	};
}