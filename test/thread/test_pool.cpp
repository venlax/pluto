//
// Created by xixi on 2025-07-13.
//
#include "thread/pool.h"

#include <iostream>

struct task_void {
    struct promise_type {
        task_void get_return_object() {
            return task_void{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> coro_;
    task_void(std::coroutine_handle<promise_type> h) : coro_(h) {}
    ~task_void() { if (coro_) coro_.destroy(); }
};

using namespace pluto;
task_void test_task(ThreadPool& pool) {
    std::cout << "main thread: " << std::this_thread::get_id() << "\n";
    co_await pool.schedule();
    std::cout << "after await: " << std::this_thread::get_id() << "\n";
}
int main() {
    ThreadPool pool(4);
    auto t = test_task(pool);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}