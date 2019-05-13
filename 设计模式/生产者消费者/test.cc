// file : main.cpp
#include <cstdio>
#include <cstdlib>
#include <thread>
#include "BlockingQueue.h"
BlockBoundQueue<int> blockboundqueue_;// 全局的缓冲边界队列
const int total = 16; // 商品总数
void consumer(size_t id, size_t n); // 消费者
void producer(size_t id, size_t n); // 生产者
int main()
{
	std::thread consumer1(consumer, 0, 5);
	std::thread consumer2(consumer, 1, 5);
	std::thread consumer3(consumer, 2, 6);
	std::thread producer1(producer, 0, 8);
	std::thread producer2(producer, 1, 8);
	consumer1.join();
	consumer2.join();
	consumer3.join();
	producer1.join();
	producer2.join();
	system("pause");
	return 0;
}
void consumer(size_t id, size_t n)
{
	for (auto i = 0; i < n; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		int item = blockboundqueue_.get();
		printf("the %d^th consumer thread has consumed the %d^th item\n", id, item);
	}
}

void producer(size_t id, size_t n)
{
	for (int i = 0; i < n; ++i)
	{
		blockboundqueue_.push(i);
		printf("the %d^th producer thread has produced the %d^th item\n", id, i);
	}
}