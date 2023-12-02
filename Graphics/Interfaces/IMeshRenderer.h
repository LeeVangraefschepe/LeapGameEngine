#pragma once

#include "mat4x4.hpp"

namespace leap::graphics
{
	class IMaterial;
	class IMesh;

	class IMeshRenderer 
	{
	public:
		virtual ~IMeshRenderer() = default;

		virtual void Draw() = 0;
		virtual void Draw(IMaterial* pMaterial) = 0;
		virtual IMaterial* GetMaterial() = 0;
		virtual void SetMaterial(IMaterial* pMaterial) = 0;
		virtual void UnsetMaterial() = 0;
		virtual void SetTransform(const glm::mat4x4& transform) = 0;
		virtual void SetMesh(IMesh* pMesh) = 0;
		virtual void UnsetMesh() = 0;
		virtual void SetIsLineRenderer(bool isLineRenderer) = 0;
	};
}