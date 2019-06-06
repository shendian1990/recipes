#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstdio>

#define _FULL 4 //


template <typename T>
class BlockBoundQueue
{
public:
	BlockBoundQueue(size_t bound = _FULL) :bound_(bound) {}
	BlockBoundQueue(const BlockBoundQueue&) = delete;
	BlockBoundQueue(BlockBoundQueue&&)= delete;
	BlockBoundQueue& operator=(const BlockBoundQueue&) = delete;
	BlockBoundQueue& operator=(BlockBoundQueue&&) = delete;
	
	void push(const T& value)
	{
		std::unique_lock<std::mutex> lck(mutex_);// 利用RAII技法，将mutex_托管给lck
		while (count_ + 1 == bound_)// 用while防止虚假唤醒 
		{
			printf("the queue is full , waiting for the consumer consuming !\n");
			notFull_.wait(lck); //等待队列非满条件发生
		}
		count_++;
		queue_.push(value);
		notEmpty_.notify_one();//通知队列非空，不能用all，不需要notifyAll通知所有的，如果有阻塞，代表原先为空，现在只生产了一个，也只要一个消费来就足够了
	}
	T get()
	{
		std::unique_lock<std::mutex> lck(mutex_);
		while (queue_.empty())
		{
			printf("the queue is empty , waiting for the producer producing !\n");
			notEmpty_.wait(lck);//等待队列为非空
		}
		T front(queue_.front());
		queue_.pop();
		count_--;
		notFull_.notify_one();//通知队列为非满，请注意不能用all
		return front;
	}
private:
	std::mutex mutex_;
	std::condition_variable notEmpty_;
	std::condition_variable notFull_;
	std::queue<T> queue_;
	size_t count_{ 0 };
	size_t bound_;
};
