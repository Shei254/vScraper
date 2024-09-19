#include "ThreadQueue.h"

template<typename T>
void ThreadQueue<T>::push(T const& val)
{
	std::lock_guard<std::mutex> lock(mutex);
	queue.push(val);
	cv.notify_one();
}

template<typename T>
T ThreadQueue<T>::pop()
{
	std::unique_lock<std::mutex> ulock(mutex);
	cv.wait(ulock, [&] { return !queue.empty(); });
	T front = queue.front();
	queue.pop();

	return front;
}

template<typename T>
bool ThreadQueue<T>::empty()
{
	return queue.size();
}
