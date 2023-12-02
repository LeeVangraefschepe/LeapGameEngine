#include "Material.h"

#include "Texture.h"
#include "../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IRenderer.h>

leap::Material::Material(const std::string& name, bool unique)
	: Material{}
{
	m_pMaterial->SetObject(ServiceLocator::GetRenderer().CloneMaterial("Default", name, !unique));
}

leap::Material& leap::Material::operator=(Material&& other) noexcept
{
	m_pMaterial = std::move(other.m_pMaterial);
	return *this;
	
}

leap::Material::Material(Material&& other) noexcept
	: m_pMaterial{ std::move(other.m_pMaterial) }
{
}

void leap::Material::SetTexture(const std::string& varName, const Texture& data) const
{
	m_pMaterial->GetUncountedObject()->SetTexture(varName, data.GetInternal());
}

void leap::Material::SetBool(const std::string& varName, bool data) const
{
	m_pMaterial->GetUncountedObject()->SetBool(varName, data);
}

void leap::Material::SetFloat(const std::string& varName, float data) const
{
	m_pMaterial->GetUncountedObject()->SetFloat(varName, data);
}

void leap::Material::SetFloat2(const std::string& varName, const glm::vec2& data) const
{
	m_pMaterial->GetUncountedObject()->SetFloat2(varName, data);
}

void leap::Material::SetFloat3(const std::string& varName, const glm::vec3& data) const
{
	m_pMaterial->GetUncountedObject()->SetFloat3(varName, data);
}

void leap::Material::SetFloat4(const std::string& varName, const glm::vec4& data) const
{
	m_pMaterial->GetUncountedObject()->SetFloat4(varName, data);
}

void leap::Material::SetMat3x3(const std::string& varName, const glm::mat3x3& data) const
{
	m_pMaterial->GetUncountedObject()->SetMat3x3(varName, data);
}

void leap::Material::SetMat4x4(const std::string& varName, const glm::mat4x4& data) const
{
	m_pMaterial->GetUncountedObject()->SetMat4x4(varName, data);
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
