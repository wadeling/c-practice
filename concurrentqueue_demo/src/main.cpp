#include <stdio.h>
#include <thread>
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

int main() {
    test_base_rw();
    printf("end\r\n");
    return 0;
}
