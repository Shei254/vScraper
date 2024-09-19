#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

#include "ThreadQueue.h"

class ThreadPool
{
private:
	int m_threads;
	std::vector<std::thread> _threads;
	ThreadQueue<std::function<void()>> _tasks;
	std::mutex _mutex;
	std::condition_variable _cv;
	bool stop;

public:
	ThreadPool(int numThreads);
	~ThreadPool();

	template<class F, class ...ARGS>
	auto ExecuteTask(F&& f, ARGS&& ...args) -> std::future<decltype(f(args...))>;
};

inline ThreadPool::ThreadPool(int numThreads) : m_threads(numThreads), stop(false)
{
	for (int i = 0; i < m_threads; i++) {
		_threads.emplace_back([this] {
			std::function<void()> task;
			while (1) {
				std::unique_lock<std::mutex> ulock(_mutex);
				_cv.wait(ulock, [this] {
					return !_tasks.empty() || stop;
					});

				if (stop) return;

				task = std::move(_tasks.pop());
				ulock.unlock();

				task();
			}
			});
	}
}

inline ThreadPool::~ThreadPool()
{
	std::unique_lock<std::mutex> _ulock;
	stop = true;
	_ulock.unlock();

	_cv.notify_all();

	for (auto& th : _threads) {
		th.join();
	}
}

template<class F, class ...ARGS>
inline auto ThreadPool::ExecuteTask(F&& f, ARGS && ...args)  -> std::future<decltype(f(args...))> {
	using return_type = decltype(f(args...));
	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<ARGS>(args)...));

	std::future<return_type> res = task->get_future();

	{
		std::unique_lock<std::mutex> ulock(_mutex);
		_tasks.push([task]() -> void { (*task)(); });
		ulock.unlock();
	}
	_cv.notify_one();

	return res;
}