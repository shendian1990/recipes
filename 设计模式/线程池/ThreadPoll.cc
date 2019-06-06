#include "ThreadPoll.h"
#include <assert.h>
#include <sstream>
#include <iostream>

using namespace muduo;

CThreadPool::CThreadPool(const string& nameArg)
	:name_(nameArg),
	maxQueueSize_(0),
	running_(false)
{
}

CThreadPool::~CThreadPool()
{
	if (running_)
	{
		stop();
	}
}

void CThreadPool::start(int numThreads)
{
	assert(threads_.empty());
	running_ = true;
	threads_.reserve(numThreads);

	for (int i = 0; i < numThreads; ++i)
	{
		// 将线程的执行过程绑定到线程池的runInThread
		threads_.push_back(new thread(&CThreadPool::runInThread, this));
	}
	if (numThreads == 0)
	{
		threadInitCallback_();
	}
}

void CThreadPool::stop()
{
	{
		lock_guard<mutex> lock(mutex_);
		running_ = false;
		// 喚醒所有的工作線程
		notEmpty_.notify_all();
	}

	//等待所有执行线程退出
	for (auto ptr_thread : threads_)
	{
		ptr_thread->join();
	}
}

size_t CThreadPool::queueSize() const
{
	lock_guard<mutex> lck(mutex_);
	return queue_.size();
}

void CThreadPool::run(const CTask& task)
{
	if (!task)
	{
		return;
	}

	if (threads_.empty())
	{
		// 当前可执行线程数为0，当前主线程子集执行这个任务；
		task();
	}
	else
	{
		unique_lock<mutex> lck(mutex_);
		notFull_.wait(lck, [&]() { return !isFull(); });

		// 将task入队，唤醒执行线程
		queue_.push_back(task);
		lck.unlock();
		notEmpty_.notify_one();
	}
}

CThreadPool::CTask CThreadPool::take()
{
	unique_lock<mutex> lock(mutex_);
	// 若线程池需要退出，也不需要再等待
	notEmpty_.wait(lock, [&]() { return !queue_.empty() || !running_; });

	CTask task;
	if (!queue_.empty())
	{
		task = queue_.front();
		queue_.pop_front();
		if (maxQueueSize_ > 0)
		{
			//条件变量的分发不需要对互斥量加锁，以免唤醒的线程再次进入wait状态
			lock.unlock();
			notFull_.notify_one();
		}
	}
	return task;
}

bool CThreadPool::isFull() const
{
	return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void CThreadPool::runInThread()
{
	try
	{
		if (threadInitCallback_)
		{
			threadInitCallback_();
		}

		while (running_)
		{
			CTask task(take());
			if (task)
			{
				task();
			}
		}
	}
	catch (...)
	{
		fprintf(stderr, "unknown exception caught in CThreadPool %s\n", name_.c_str());
		throw; // rethrow
	}
}

// 测试代码
// 1. 执行函数定义
#define Func(index) \
void func_##index() \
{\
    ostringstream ss;\
    ss << "=========== thread: " << std::this_thread::get_id() << std::endl;\
    std::cout << ss.str() << std::endl;\
    std::cout.flush();\
}\

Func(1)
Func(2)

//2. 执行任务入队列
#define RUN_IN_POOL(index) \
do{ tp.run(std::function<void()>(&func_##index)); }while(0)

void main()
{
	CThreadPool tp;
	tp.start(2);
	tp.setMaxQueueSize(3);

	RUN_IN_POOL(1);
	RUN_IN_POOL(2);

	// 睡眠一下，否则立即调用stop，部分任务还未执行完
	Sleep(1000);
	tp.stop();
	system("pause");
}