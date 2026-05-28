#include <coroutine>
#include <exception>
#include <future>
#include <thread>
#include <type_traits>

namespace edge::coroutine_support
{
    template< typename R> 
    struct promise;

    template< typename R> 
    struct coroutine : std::coroutine_handle<promise<R>>
    {
        using promise_type = struct promise<R>;
    };

    template<> 
    struct coroutine<void> : std::coroutine_handle<promise<void>>
    {
        using promise_type = struct promise<void>;
    };

    template< typename R> 
    struct promise : std::promise<R>
    {
        coroutine<R> get_return_object() { return {coroutine<R>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_value(const R& value)
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            this->set_value(value);
        }

        void return_value(R&& value) noexcept(std::is_nothrow_move_constructible_v<R>)
        {
            this->set_value(std::move(value));
        }
        void unhandled_exception() {}
    };

    template<> 
    struct promise<void> : std::promise<void>
    {
        coroutine<void> get_return_object() { return {coroutine<void>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void(){}
        void unhandled_exception() {}
    };

    template< typename...>
    struct awaitable;

    template< typename R, typename... Args>
    struct awaitable<R,Args...>
    {
    awaitable(R(&func)(Args...), Args... args)
    : _func([&]()->R{ return func(args...); })
    {}
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<R>){}
        void await_suspend(std::coroutine_handle<>){}
        R await_resume() { return _func();}

        std::function<R()> _func;
    };

    template<typename... Args>
    struct awaitable<void,Args...>
    {
    awaitable(void(&func)(Args...), Args... args)
    : _func([&](){ func(args...); })
    {}
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<>){}
        void await_resume() { _func();}

        std::function<void()> _func;
    };

    template< typename R, typename... Args>
    auto coro_call(R(&f)(Args...), Args... args)
    {
        return awaitable<R,Args...>(f,args...);
    }

    template<typename... Args>
    auto coro_call(void(&f)(Args...), Args... args)
    {
        return awaitable<void,Args...>(f,args...);
    }
}
