#include "Material.h"

#include "../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IRenderer.h>

leap::Material::Material(const std::string& name, bool unique)
	: Material{}
{
	m_pMaterial->SetObject(ServiceLocator::GetRenderer().CloneMaterial("Default", name, !unique));
}

leap::graphics::IMaterial* leap::Material::GetInternal() const
{
	return m_pMaterial->GetObject();
}

leap::Material::Material()
{
	m_pMaterial = std::make_unique<GraphicsObject<graphics::IMaterial>>();
}

void leap::Material::CreateShaderMaterial(const std::string& name, std::unique_ptr<graphics::Shader, graphics::ShaderDelete> pShader, bool unique)
{
	m_pMaterial->SetObject(ServiceLocator::GetRenderer().CreateMaterial(std::move(pShader), name, !unique));
}
