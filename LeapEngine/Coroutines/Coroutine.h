#pragma once
#include <coroutine>

namespace leap
{
    template<typename T>
    struct Coroutine;

    using IEnumerator = Coroutine<bool>;

    template<typename T = IEnumerator>
    struct Coroutine
    {
        struct Promise;
        using Handle_type = std::coroutine_handle<Promise>;
        using promise_type = Promise;

        struct Promise
        {
            T value{};

            Coroutine get_return_object()
            {
                return (Handle_type::from_promise(*this));
            }
            auto initial_suspend()
            {
                return std::suspend_never{};
            }
            auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            void unhandled_exception() {}

            template<std::convertible_to<T> From> // C++20 concept
            auto yield_value(From&& from)
            {
                value = std::forward<From>(from); // caching the result in promise
                return std::suspend_always{};
            }
            void return_void() {}
        };

        Handle_type handle;
        bool IsValid() const { return m_IsValid; }

        Coroutine()
            :m_IsValid{false}
        {
        }
        Coroutine(Handle_type h)
            :handle(h)
        {
        }
        ~Coroutine()
        {
            handle.destroy();
        }
        bool IsDone() const noexcept
        {
            return handle.done();
        }
        void Resume()
        {
            handle();
        }
        T Value()
        {
            return std::move(handle.promise().value);
        }

    private:
        bool m_IsValid{ true };
    };
}