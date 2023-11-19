#pragma once

namespace leap::graphics
{
	class CustomMesh;

	class IMesh
	{
	public:
		virtual ~IMesh() = default;

		virtual void ReloadMesh(const CustomMesh& mesh) = 0;
	};
}