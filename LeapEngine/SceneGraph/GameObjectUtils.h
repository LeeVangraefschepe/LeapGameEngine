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
	}

	// Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way
	template <typename T>
	constexpr std::string_view ConstexprTypeName()
	{
		constexpr std::string_view wrappedName(Detail::WrappedTypeName<T>());

		constexpr size_t endOfType{ wrappedName.find_last_of('>') };
		constexpr size_t beginOfType{ Math::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

		return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
	}

	constexpr int ConstexprStringHash(const std::string_view String)
	{
		return std::hash<std::string_view>{}(String);
	}

	template<typename T>
	constexpr int GenerateComponentID()
	{
		constexpr std::string_view typeName(ConstexprTypeName<T>());

		return ConstexprStringHash(String);
	}
}