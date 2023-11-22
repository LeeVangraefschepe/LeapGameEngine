#pragma once

#include "GraphicsObject.h"

#include <Interfaces/ITexture.h>

#include <Subject.h>

#include <string>
#include <memory>

namespace leap
{
	namespace graphics
	{
		class ITexture;
	}

	class Texture final
	{
	public:
		Texture();
		Texture(const std::string& filePath, bool unique = false);
		~Texture() = default;

		Texture(const Texture& mesh) = delete;
		Texture& operator=(const Texture& mesh) = delete;
		Texture(Texture&& mesh) noexcept;
		Texture& operator=(Texture&& mesh) noexcept;

		void Load(const std::string& filePath, bool unique = false);
		void Load(unsigned int width, unsigned int height);
		void SetData(const std::vector<unsigned char>& data) const;

		glm::uvec2 GetSize() const;
		std::vector<unsigned char> GetData() const;

		graphics::ITexture* GetInternal() const;

		Subject OnInternalChange{};

	private:
		std::unique_ptr<GraphicsObject<graphics::ITexture>> m_pObject{};
	};
}