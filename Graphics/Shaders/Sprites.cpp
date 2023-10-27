#include "Sprites.h"

#include "../Shader.h"

std::unique_ptr<leap::graphics::Shader, leap::graphics::ShaderDelete> leap::graphics::shaders::Sprites::GetShader()
{
	auto pShader{ std::unique_ptr<Shader, ShaderDelete>{ new Shader{}, ShaderDelete{} } };
	pShader->directXVertexData = []()
		{
			// Create vertex layout
			std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc;

			static constexpr unsigned int numElements{ 4 };
			vertexDesc.resize(numElements);

			vertexDesc[0].SemanticName = "POSITION";
			vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertexDesc[0].AlignedByteOffset = 0;
			vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			vertexDesc[1].SemanticName = "PIVOT";
			vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			vertexDesc[1].AlignedByteOffset = 12;
			vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			vertexDesc[2].SemanticName = "SIZE";
			vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
			vertexDesc[2].AlignedByteOffset = 20;
			vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			vertexDesc[3].SemanticName = "COLOR";
			vertexDesc[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			vertexDesc[3].AlignedByteOffset = 28;
			vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			return vertexDesc;
		};
	pShader->directXDataPath = "Data/Engine/Shaders/Sprites.fx";

	return std::move(pShader);
}
