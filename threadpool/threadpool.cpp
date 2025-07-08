#include "threadpool.h"
#include <iostream>

ThreadPool::ThreadPool(int min, int max) : m_minNum(min), m_maxNum(max)
{
    m_idleCount = m_curCount = min;
    std::cout << "线程数量: " << m_curCount << std::endl;
    m_manager = std::make_shared<std::thread>(&ThreadPool::manager, this);
    for (int i = 0; i < min; ++i)
    {
        auto t = std::make_shared<std::thread>(&ThreadPool::worker, this);
        m_workers[t->get_id()] = t;
    }
}

ThreadPool::~ThreadPool()
{
    ShutDown();
}

void ThreadPool::AddTask(std::shared_ptr<BaseTask> t)
{
    {
        std::lock_guard<std::mutex> lock(m_mutexTasks);
        m_taskQ.push(t);
    }
    m_cv.notify_one();
}

void ThreadPool::ShutDown()
{
    m_shutDown = true;
    m_cv.notify_all();
    for (auto t : m_workers)
    {
        if (t.second->joinable())
        {
            std::cout << "******** 线程 " << t.second->get_id() << " 将要退出了..." << std::endl;
            t.second->join();
        }
    }
    if (m_manager->joinable())
    {
        m_manager->join();
    }
}

void ThreadPool::worker()
{
    while (!m_shutDown)
    {
        std::unique_lock<std::mutex> lock(m_mutexTasks);
        while (!m_shutDown && m_taskQ.empty() && m_exitCount.load() <= 0)
        {
            m_cv.wait(lock);
        }

        if (m_shutDown)
        {
            return;
        }

        if (m_exitCount.load() > 0)
        {
            m_exitCount--;
            m_curCount--;
            m_idleCount--;
            std::unique_lock<std::mutex> lock(m_mutexDels);
            m_delThreads.push_back(std::this_thread::get_id());
            return;
        }

        std::shared_ptr<BaseTask> t = nullptr;
        if (!m_taskQ.empty())
        {
            t = m_taskQ.front();
            m_taskQ.pop();
        }
        lock.unlock();

        if (t != nullptr)
        {
            m_idleCount--;
            t->Run();
            m_idleCount++;
        }
    }
}

void ThreadPool::manager()
{
    while (!m_shutDown)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        recycleThreads();
        int idle = m_idleCount.load();
        int cur = m_curCount.load();
        std::cout << "idle=" << idle << "count=" << cur << std::endl;

        if (idle > cur / 2 && cur > m_minNum)
        {
            m_exitCount.store(1);
            m_cv.notify_all();
        }
        else if (idle == 0 && cur < m_maxNum)
        {
            auto t = std::make_shared<std::thread>(&ThreadPool::worker, this);
            std::cout << "+++++++++++++++ 添加了一个线程, id: " << t->get_id() << std::endl;
            m_workers[t->get_id()] = t;
            m_curCount++;
            m_idleCount++;
        }
    }
}

void ThreadPool::recycleThreads()
{
    std::unique_lock<std::mutex> lck(m_mutexDels);
    for (auto &id : m_delThreads)
    {
        auto it = m_workers.find(id);
        if (it != m_workers.end())
        {
            std::cout << "+++++++++++++++ 销毁了一个线程, id: " << id << std::endl;
            if (it->second->joinable())
            {
                it->second->join();
            }
            m_workers.erase(it);
        }
    }
    m_delThreads.clear();
}
