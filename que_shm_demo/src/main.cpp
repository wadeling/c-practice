#include <stdio.h>
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include "que_shm.h"

using namespace std;


int main() {
    int result;
    std::string value="aaa";
    CQueShm test_que("test_que",1000,10,10);
    result = test_que.Put(value);

    std::string value2;
    int pop_result;
    pop_result = test_que.Pop(value2);
 
    printf("pop %s\r\n",value2.c_str());
    printf("end\r\n");
    return 0;
}
