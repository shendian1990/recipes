#pragma once

#include <deque>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

namespace muduo
{
	using namespace std;

	class CThreadPool
	{
	public:
		// 入参为空的任务类型
		typedef function<void()> CTask;

		// 构造及析构函数
		explicit CThreadPool(const string& nameArg = string("ThreadPool"));
		~CThreadPool();

		// 设置任务队列的数量上限
		void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
		// 设置工作线程初次执行时的消息回调
		void setThreadInitCallback(const CTask& cb)
		{
			threadInitCallback_ = cb;
		}

		// 开启指定数量的工作线程
		void start(int numThreads);
		// 退出线程池执行，退出所有的工作线程
		void stop();

		// 线程池名称
		const string& name() const
		{
			return name_;
		}

		// 任务队列多少
		size_t queueSize() const;

		// 新任务入队，若队列已满，将堵塞接口调用线程
		void run(const CTask& f);

	private:
		// 禁止拷贝构造
		CThreadPool(const CThreadPool&) = delete;
		CThreadPool& operator=(const CThreadPool&) = delete;

		// 判断队列是否已满，可采用lambda表达式替代
		bool isFull() const;
		// 完成任务到线程的分发
		// 线程池对象维护了任务队列，工作线程在执行时需要从任务队列中获取工作任务，
		// 所以只能将线程的执行体与线程池对象的接口关联，完成工作任务获取；
		void runInThread();
		// 从任务队列中取任务执行
		CThreadPool::CTask take();

	private:
		mutable mutex mutex_; // 任务队列的互斥锁
		condition_variable notEmpty_; // 当前队列为空的条件变量，工作线程取任务时空则等待；
		condition_variable notFull_;  // 队列已满的条件变量，任务如队时，队列满则等待；
		string name_; // 线程池名称
		CTask threadInitCallback_; // 工作线程初次执行的回调
		vector<thread*> threads_;  // 工作线程集
		deque<CTask> queue_;  // 任务队列
		size_t maxQueueSize_; //任务量上限
		bool running_; // 线程池是否在执行
	};
}