#pragma once

namespace leap::physics
{
	enum class EShape
	{
		Box,
		Sphere
	};

	class IShape
	{
	public:
		virtual ~IShape() = default;
	};
}