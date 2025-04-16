//
// Created by f1rset on 4/2/25.
//

#ifndef INTEGRATE_PARALLEL_TPOOL_THREADPOOL_HPP
#define INTEGRATE_PARALLEL_TPOOL_THREADPOOL_HPP
#include <functional>
#include <thread>
#include <numeric>
#include <future>
#include <queue>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(
                [func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
                    return func(std::forward<Args>(args)...);
                }
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};



#endif //INTEGRATE_PARALLEL_TPOOL_THREADPOOL_HPP
