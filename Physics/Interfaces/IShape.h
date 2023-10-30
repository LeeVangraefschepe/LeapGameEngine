#pragma once

namespace leap::physics
{
	enum class EShape
	{
		Box
	};

	class IShape
	{
	public:
		virtual ~IShape() = default;
	};
}