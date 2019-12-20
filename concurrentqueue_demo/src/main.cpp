#include <stdio.h>
#include <thread>
#include <iostream>
#include <chrono>

#include "concurrentqueue.h"

using namespace std;


void test_base_rw() {
	moodycamel::ConcurrentQueue<int> q;
	q.enqueue(25);
	
	int item;
	bool found = q.try_dequeue(item);
	assert(found && item == 25);
	printf("test end\r\n");
}
void test_multi_thread_rw() {
    printf("test concurrentQueue  \r\n");
    moodycamel::ConcurrentQueue<int> q;
    int dequeued[100] = { 0 };
    int thread_num = 5;
    std::thread producer_threads[5];
    std::thread consumer_threads[5];

    // Producers
    printf("producers start.\r\n");
    for (int i = 0; i != thread_num; ++i) {
        producer_threads[i] = std::thread([&](int i) {
            for (int j = 0; j != thread_num; ++j) {
                q.enqueue(i * thread_num + j);
            }
            printf("producer[%d] enqueue end.\r\n",i);
        }, i);
    }

    // Consumers
    printf("consumer start.\r\n");
    for (int i = 0; i != thread_num; ++i) {
        consumer_threads[i] = std::thread([&]() {
            int item;
            int count = 0;
            for (int j = 0; j != 20; ++j) {
                if (q.try_dequeue(item)) {
                    ++dequeued[item];
                    count++;
                }
            }
            printf("consumer[%d] dequeue num %d\r\n",i,count);
        });
    }

    // Wait for all threads
    for (int i = 0; i != 5; ++i) {
        producer_threads[i].join();
        consumer_threads[i].join();
    }

    // Collect any leftovers (could be some if e.g. consumers finish before producers)
    printf("consumer and producer done,check any left.\r\n");
    int item;
    while (q.try_dequeue(item)) {
        printf("last dequeue %d\r\n",item);
        ++dequeued[item];
    }

    // Make sure everything went in and came back out!
    for (int i = 0; i != 25; ++i) {
        assert(dequeued[i] == 1);
    }
}


int main() {
    //test_base_rw();

    test_multi_thread_rw();

    std::cout << "done!\n";
    return 0;
}
