#pragma once

#include "mat4x4.hpp"

#include <string>

namespace leap::graphics
{
	class IMaterial;
	class CustomMesh;

	class IMeshRenderer 
	{
	public:
		virtual ~IMeshRenderer() = default;

		virtual void Draw() = 0;
		virtual void Draw(IMaterial* pMaterial) = 0;
		virtual IMaterial* GetMaterial() = 0;
		virtual void SetMaterial(IMaterial* pMaterial) = 0;
		virtual void SetTransform(const glm::mat4x4& transform) = 0;
		virtual void LoadMesh(const std::string& filePath) = 0;
		virtual void LoadMesh(const CustomMesh& mesh) = 0;
		virtual void SetIsLineRenderer(bool isLineRenderer) = 0;
	};
}