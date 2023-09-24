#pragma once

#include <d3dx11effect.h>

#include <vector>
#include <functional>
#include <string>

namespace leap::graphics
{
	struct Shader final
	{
		std::function<std::vector<D3D11_INPUT_ELEMENT_DESC>()> directXVertexData{};
		std::string directXDataPath{};
	};
}