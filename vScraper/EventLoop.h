#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <future>
#include <condition_variable>

class EventLoop
{
	using callable_t = std::function<void()>;

private:
	std::vector<callable_t> _writeBuffer;
	std::mutex _mutex;
	std::condition_variable _cv;
	bool _running = true;
	std::thread _thread { &EventLoop::threadFunc, this };

	void threadFunc() noexcept;
public:
	EventLoop() = default;
	EventLoop(const EventLoop&) = delete;
	EventLoop(EventLoop&&) noexcept = delete;

	EventLoop& operator = (const EventLoop&) = delete;
	EventLoop& operator = (EventLoop&&) = delete;
	~EventLoop();

	void enqueue(callable_t&& callable) noexcept;
};

