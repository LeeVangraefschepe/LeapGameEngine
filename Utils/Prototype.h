#pragma once
#include <memory>

namespace leap
{
	// Inherit this interface for the base class.
	// This is a CRTP, the template argument should be the base class.
	template <typename T>
	class Prototype
	{
	public:
		virtual std::unique_ptr<T> Clone() = 0;
	};
}