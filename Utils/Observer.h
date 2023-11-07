#pragma once

namespace leap
{
	class Subject;
	template<typename T>
	class TSubject;

	class Observer
	{
	public:
		virtual ~Observer() = default;

	protected:
		virtual void Notify() = 0;
		virtual void OnSubjectDestroy() {};

		friend Subject;
	};

	template <typename T>
	class TObserver
	{
	public:
		virtual ~TObserver() = default;

	protected:
		virtual void Notify(const T& data) = 0;
		virtual void OnSubjectDestroy() {};

		friend TSubject<T>;
	};
}