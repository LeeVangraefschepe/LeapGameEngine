#include "Texture.h"

#include "../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>

leap::Texture::Texture()
	: m_pObject{ std::make_unique<GraphicsObject<graphics::ITexture>>() }
{
}

leap::Texture::Texture(const std::string& filePath, bool unique)
	: Texture{}
{
	Load(filePath, unique);
}

leap::Texture::Texture(Texture&& mesh) noexcept
	: m_pObject{ std::move(mesh.m_pObject) }
{
}

leap::Texture& leap::Texture::operator=(Texture&& mesh) noexcept
{
	m_pObject = std::move(mesh.m_pObject);

	return *this;
}

void leap::Texture::Load(const std::string& filePath, bool unique)
{
	// Load texture
	graphics::ITexture* pTexture{ ServiceLocator::GetRenderer().CreateTexture(filePath, !unique) };
	m_pObject->SetObject(pTexture);
	OnInternalChange.Notify();
}

void leap::Texture::Load(unsigned int width, unsigned int height)
{
	graphics::ITexture* pTexture{ ServiceLocator::GetRenderer().CreateTexture(width, height) };
	m_pObject->SetObject(pTexture);
	OnInternalChange.Notify();
}

void leap::Texture::SetData(const std::vector<unsigned char>& data) const
{
	m_pObject->GetUncountedObject()->SetData(data);
}

glm::uvec2 leap::Texture::GetSize() const
{
	if (!m_pObject->GetUncountedObject()) return {};
	return m_pObject->GetUncountedObject()->GetSize();
}

std::vector<unsigned char> leap::Texture::GetData() const
{
	if (!m_pObject->GetUncountedObject()) return {};
	return m_pObject->GetUncountedObject()->GetData();
}

leap::graphics::ITexture* leap::Texture::GetInternal() const
{
	return m_pObject->GetObject();
}
