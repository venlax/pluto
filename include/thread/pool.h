//
// Created by xixi on 2025-07-13.
//

#ifndef PLUTO_THREAD_POOL_H
#define PLUTO_THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <coroutine>
#include <functional>
#include <future>
#include <queue>
#include <thread>

namespace pluto {
    class ThreadPool {
    public:
        explicit ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency())
            : stop_flag_(false) {
            if (thread_count == 0) {
                thread_count = 1;
            }
            workers_.reserve(thread_count);
            for (std::size_t i = 0; i < thread_count; ++i) {
                workers_.emplace_back([this]() {
                    while (true) {
                        Task task;
                        {
                            std::unique_lock lock(queue_mutex_);
                            cv_.wait(lock, [this] {return stop_flag_.load() || !tasks_.empty();});
                            if (stop_flag_.load() && tasks_.empty()) {
                                return;
                            }
                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                        task();
                    }
                });
            }
        }

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

        ThreadPool(ThreadPool&&) = default;
        ThreadPool& operator=(ThreadPool&&) = default;

        ~ThreadPool() {shutdown();}

        template <typename F, typename... Args>
        [[nodiscard]] auto enqueue(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
            using return_type = std::invoke_result_t<F, Args...>;
            auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f),std::forward<Args>(args)...));
            {
                std::lock_guard lock(queue_mutex_);
                if (stop_flag_.load()) {
                    // TODO
                }
                tasks_.emplace([task_ptr] { (*task_ptr)(); });
            }
            cv_.notify_one();
            return task_ptr->get_future();
        }


        void shutdown() {
            if (!stop_flag_.exchange(true)) {
                {
                    std::lock_guard lock(queue_mutex_);
                }
                cv_.notify_all();
                for (auto& worker : workers_) {
                    if (worker.joinable()) {
                        worker.join();
                    }
                }
            }
        }

        struct ScheduleAwaiter {
            ThreadPool& pool_;
            std::coroutine_handle<> handle_;

            bool await_ready() const noexcept {
                return false;
            }

            void await_suspend(std::coroutine_handle<> h) {
                handle_ = h;
                auto ft = pool_.enqueue([this_handle = handle_] {
                    this_handle.resume();
                });
            }
            void await_resume() const noexcept {}
        };

        ScheduleAwaiter schedule() noexcept {
            return ScheduleAwaiter{*this};
        }

    private:
        using Task = std::function<void()>;

        std::vector<std::thread> workers_;

        std::queue<Task> tasks_;

        std::mutex queue_mutex_;

        std::condition_variable cv_;

        std::atomic<bool> stop_flag_;

    };
}


// A simple task coroutine wrapper
// struct task_void {
//     struct promise_type {
//         task_void get_return_object() {
//             return task_void{std::coroutine_handle<promise_type>::from_promise(*this)};
//         }
//         std::suspend_never initial_suspend() noexcept { return {}; }
//         std::suspend_always final_suspend() noexcept { return {}; }
//         void return_void() noexcept {}
//         void unhandled_exception() { std::terminate(); }
//     };
//
//     std::coroutine_handle<promise_type> coro_;
//     task_void(std::coroutine_handle<promise_type> h) : coro_(h) {}
//     ~task_void() { if (coro_) coro_.destroy(); }
// };

// Usage Example
// #include "thread_pool.h"
// using namespace pluto;
// task_void test_task(ThreadPool& pool) {
//     std::cout << "main thread: " << std::this_thread::get_id() << "\n";
//     co_await pool.schedule();
//     std::cout << "after await: " << std::this_thread::get_id() << "\n";
// }
// int main() {
//     ThreadPool pool(4);
//     test_task(pool);
//     std::this_thread::sleep_for(std::chrono::seconds(1));
// }
#endif //PLUTO_THREAD_POOL_H
