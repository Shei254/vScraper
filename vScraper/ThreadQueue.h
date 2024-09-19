#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

template<typename T>
class ThreadQueue
{
private:
	std::queue<T> queue;
	std::condition_variable cv;
	std::mutex mutex;
public:
	void push(T const& val);
	T pop();
	bool empty();
};

