#include <iostream>
#include "EventLoop.h"

void EventLoop::threadFunc() noexcept
{
	std::vector<callable_t> _readBuffer;
	while (_running) {
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [this] {
				return !_writeBuffer.empty() || !_running;
			});

			std::swap(_readBuffer, _writeBuffer);
		}

		for (callable_t& func : _readBuffer) {
			try
			{
				func();
			}
			catch (const std::exception& e)
			{
				std::cout << "[-] Something went wrong running task: " << e.what() << std::endl;
			}
		}

		_readBuffer.clear();
	}
}

EventLoop::~EventLoop() noexcept
{
	enqueue([this] {
		_running = false;
	});

	if (_thread.joinable()) {
		_thread.join();
	}
}

void EventLoop::enqueue(callable_t&& callable) noexcept
{
	{
		std::lock_guard<std::mutex> guard(_mutex);
		_writeBuffer.emplace_back(std::move(callable));
	}
	_cv.notify_one();
}
