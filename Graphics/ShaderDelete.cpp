#include "ShaderDelete.h"
#include "Shader.h"

void leap::graphics::ShaderDelete::operator()(Shader* pShader) const
{
	delete pShader;
}
