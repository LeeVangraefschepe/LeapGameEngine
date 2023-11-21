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
}

glm::ivec2 leap::Texture::GetSize() const
{
	return m_pObject->GetUncountedObject()->GetSize();
}

std::vector<unsigned char> leap::Texture::GetData() const
{
	return m_pObject->GetUncountedObject()->GetData();
}

leap::graphics::ITexture* leap::Texture::GetInternal() const
{
	return m_pObject->GetObject();
}
