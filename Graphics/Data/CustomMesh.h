#pragma once

#include "Vertex.h"

#include <vector>

namespace leap::graphics
{
	struct CustomMesh final
	{
		std::vector<Vertex> vertices{};
		std::vector<unsigned int> indices{};
	};
}