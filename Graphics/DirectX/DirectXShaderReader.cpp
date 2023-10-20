#include "DirectXShaderReader.h"

#include "../Shader.h"

leap::graphics::DirectXShader leap::graphics::DirectXShaderReader::GetShaderData(std::unique_ptr<Shader, ShaderDelete> pShader)
{
	DirectXShader shader{};
	shader.path = pShader->directXDataPath;
	shader.vertexDataFunction = pShader->directXVertexData;
	return shader;
}
