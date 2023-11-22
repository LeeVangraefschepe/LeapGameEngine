#pragma once

#include "Shader.h"
#include "GraphicsObject.h"

#include <Interfaces/IMaterial.h>

#include <memory>

namespace leap
{
	class Texture;
	namespace graphics
	{
		struct Shader;
	}

	class Material final
	{
	public:
		Material(const std::string& name, bool unique = false);

		template<typename T>
		Material(const std::string& name, const Shader<T>& shader, bool unique = false)
			: Material{}
		{
			CreateShaderMaterial(name, shader.GetShader(), unique);
		}

		template<typename T>
		void Set(const std::string& varName, const T& data) const
		{
			if constexpr (std::is_same_v<T, Texture>)
			{
				SetTexture(varName, data);
			}
		}

		void SetTexture(const std::string& varName, const Texture& data) const;

		graphics::IMaterial* GetInternal() const;

	private:
		Material();
		void CreateShaderMaterial(const std::string& name, std::unique_ptr<graphics::Shader, graphics::ShaderDelete> pShader, bool unique);

		std::unique_ptr<GraphicsObject<graphics::IMaterial>> m_pMaterial{};
	};
}