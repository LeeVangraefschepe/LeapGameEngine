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
		Material& operator=(const Material& other) = delete;
		Material(const Material& other) = delete;
		Material& operator=(Material&& other) noexcept;
		Material(Material&& other) noexcept;

		template<typename T>
		static Material Create(const std::string& name, bool unique = false)
		{
			Material m{};
			m.CreateShaderMaterial(name, T::GetShader(), unique);
			return m;
		}

		template<typename T>
		static std::unique_ptr<Material> CreatePtr(const std::string& name, bool unique = false)
		{
			std::unique_ptr<Material> m{ std::unique_ptr<Material>(new Material{}) };
			m->CreateShaderMaterial(name, T::GetShader(), unique);
			return std::move(m);
		}

		template<typename T>
		void Set(const std::string& varName, const T& data) const
		{
			if constexpr (std::is_same_v<T, Texture>)
			{
				SetTexture(varName, data);
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				SetFloat(varName, data);
			}
			else if constexpr (std::is_same_v<T, glm::vec2>)
			{
				SetFloat2(varName, data);
			}
			else if constexpr (std::is_same_v<T, glm::vec3>)
			{
				SetFloat3(varName, data);
			}
			else if constexpr (std::is_same_v<T, glm::vec4>)
			{
				SetFloat4(varName, data);
			}
			else if constexpr (std::is_same_v<T, glm::mat3x3>)
			{
				SetMat3x3(varName, data);
			}
			else if constexpr (std::is_same_v<T, glm::mat4x4>)
			{
				SetMat4x4(varName, data);
			}
		}

		void SetTexture(const std::string& varName, const Texture& data) const;
		void SetBool(const std::string& varName, bool data) const;
		void SetFloat(const std::string& varName, float data) const;
		void SetFloat2(const std::string& varName, const glm::vec2& data) const;
		void SetFloat3(const std::string& varName, const glm::vec3& data) const;
		void SetFloat4(const std::string& varName, const glm::vec4& data) const;
		void SetMat3x3(const std::string& varName, const glm::mat3x3& data) const;
		void SetMat4x4(const std::string& varName, const glm::mat4x4& data) const;

		graphics::IMaterial* GetInternal() const;

	private:
		Material();
		void CreateShaderMaterial(const std::string& name, std::unique_ptr<graphics::Shader, graphics::ShaderDelete> pShader, bool unique);

		std::unique_ptr<GraphicsObject<graphics::IMaterial>> m_pMaterial{};
	};
}