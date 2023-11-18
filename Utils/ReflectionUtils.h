#pragma once

#include <algorithm>
#include <string_view>

#ifdef max
#undef max
#endif

namespace leap::ReflectionUtils
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
	}

	// Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way
	template <typename T>
	constexpr std::string_view ConstexprTypeName()
	{
		constexpr std::string_view wrappedName{ Detail::WrappedTypename<T>() };

		constexpr size_t endOfType{ wrappedName.find_last_of('>') };
		// I would use std::max, but it doesn't work for some inexplicable reason
		constexpr size_t beginOfType{ std::max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

		return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
	}

	// Reference: https://www.partow.net/programming/hashfunctions/index.html#AvailableHashFunctions
	constexpr unsigned int ConstexprStringHash(const char* str, unsigned int length)
	{
		unsigned int hash = 1315423911;

		for (unsigned int i = 0; i < length; ++str, ++i)
		{
			hash ^= ((hash << 5) + (*str) + (hash >> 2));
		}

		return hash;
	}

	template<typename T>
	constexpr unsigned int GenerateTypenameHash()
	{
		constexpr std::string_view Typename(ConstexprTypeName<T>());

		return ConstexprStringHash(Typename.data(), static_cast<unsigned int>(Typename.size()));
	}
}