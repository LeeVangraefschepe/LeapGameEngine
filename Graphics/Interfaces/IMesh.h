#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Quaternion.h>

namespace leap::graphics
{
	class CustomMesh;

	class IMesh
	{
	public:
		virtual ~IMesh() = default;

		virtual const std::vector<std::unique_ptr<IMesh>>& GetSubMeshes() = 0;
		virtual const std::string& GetName() = 0;
		virtual void ReloadMesh(const CustomMesh& mesh) = 0;
		virtual void Remove() = 0;
		/// <summary>
		/// Return whether or not this mesh have any vertices stored in it
		/// </summary>
		virtual bool IsValid() = 0;
		virtual const glm::vec3& GetPosition() = 0;
		virtual const glm::quat& GetRotation() = 0;
		virtual const glm::vec3& GetScale() = 0;
	};
}