#pragma once

#include <string>
#include <vector>

#include "Vertex.h"

#include <Vector3.h>
#include <Quaternion.h>

namespace leap::graphics
{
	struct MeshData final
	{
		std::string name{};
		std::vector<std::vector<Vertex>> vertexBuffers{};
		std::vector<std::vector<unsigned int>> indexBuffers{};
		glm::vec3 position{};
		glm::quat rotation{ Quaternion::Identity() };
		glm::vec3 scale{ Vector3::One() };
		std::vector<MeshData> subMeshes{};
	};
}