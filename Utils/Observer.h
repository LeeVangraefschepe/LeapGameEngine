#pragma once

namespace leap
{
	template <typename T>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const T& data) = 0;
		virtual void OnSubjectDestroy() {};
	};
}