#pragma once

#include <string_view>

namespace leap::GOutils
{
	namespace Detail
	{
		template<typename T>
		constexpr std::string_view WrappedTypename()
		{
			#if _MSC_VER
			return __FUNCSIG__;
			#else // GCC and Clang use __PRETTY_FUNCTION__
			return __PRETTY_FUNCTION__;
			#endif
		}

		constexpr size_t Max(const size_t A, const size_t B)
		{
			return A < B ? B : A;
		}
	}

	// Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way
	template <typename T>
	constexpr std::string_view ConstexprTypeName()
	{
		constexpr std::string_view wrappedName{ Detail::WrappedTypename<T>() };

		constexpr size_t endOfType{ wrappedName.find_last_of('>') };
		// I would use std::max, but it doesn't work for some inexplicable reason
		constexpr size_t beginOfType{ Detail::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

		return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
	}

	// Reference: https://www.partow.net/programming/hashfunctions/index.html#AvailableHashFunctions
	constexpr uint32_t ConstexprStringHash(const char* str, uint32_t length)
	{
		uint32_t hash = 1315423911;

		for (uint32_t i = 0; i < length; ++str, ++i)
		{
			hash ^= ((hash << 5) + (*str) + (hash >> 2));
		}

		return hash;
	}

	template<typename T>
	constexpr uint32_t GenerateComponentID()
	{
		constexpr std::string_view Typename(ConstexprTypeName<T>());

		return ConstexprStringHash(Typename.data(), static_cast<uint32_t>(Typename.size()));
	}
}