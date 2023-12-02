#pragma once

#include "mat4x4.hpp"

#include <string>
#include <any>

namespace leap::graphics
{
	class ITexture;

	class IMaterial
	{
	public:
		virtual ~IMaterial() = default;

		virtual void SetBool(const std::string& varName, bool data) = 0;
		virtual void SetFloat(const std::string& varName, float data) = 0;
		virtual void SetFloat2(const std::string& varName, const glm::vec2& data) = 0;
		virtual void SetFloat3(const std::string& varName, const glm::vec3& data) = 0;
		virtual void SetFloat4(const std::string& varName, const glm::vec4& data) = 0;
		virtual void SetMat3x3(const std::string& varName, const glm::mat3x3& data) = 0;
		virtual void SetMat4x4(const std::string& varName, const glm::mat4x4& data) = 0;
		virtual void SetTexture(const std::string& varName, ITexture* pTexture) = 0;

		virtual void Remove() = 0;
	};
}