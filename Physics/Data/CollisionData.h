#pragma once

namespace leap::physics
{
	struct CollisionData final
	{
		void* pFirst{};
		void* pSecond{};
	};
}