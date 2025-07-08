#pragma once
#include <map>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <tuple>
#include <condition_variable>
#include <atomic>

class BaseTask
{
public:
    virtual void Run() const = 0;
    virtual ~BaseTask() = default;
};

template <typename... Args>
class Task : public BaseTask
{
public:
    Task(std::function<void(Args...)> func, Args... args) : m_func(func), m_args(std::make_tuple(args...))
    {
    }
    void Run() const
    {
        if (m_func)
        {
            std::apply(m_func, m_args); // 调用任务函数并传递参数
        }
    }

private:
    std::function<void(Args...)> m_func; // 任务函数
    std::tuple<Args...> m_args;          // 任务参数
};

class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

public:
    void AddTask(std::shared_ptr<BaseTask> t);
    void ShutDown();

private:
    void worker();
    void manager();
    void recycleThreads();

private:
    std::shared_ptr<std::thread> m_manager{};

    std::queue<std::shared_ptr<BaseTask>> m_taskQ;
    std::mutex m_mutexTasks;

    std::map<std::thread::id, std::shared_ptr<std::thread>> m_workers;
    std::condition_variable m_cv;

    std::vector<std::thread::id> m_delThreads;
    std::mutex m_mutexDels;

    int m_minNum{};
    int m_maxNum{};

    std::atomic<bool> m_shutDown{};

    std::atomic<int> m_curCount{};  // 当前线程数
    std::atomic<int> m_idleCount{}; // 空闲线程数
    std::atomic<int> m_exitCount{}; // 需要退出的线程数
};
