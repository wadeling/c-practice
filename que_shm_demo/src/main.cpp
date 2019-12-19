#include <stdio.h>
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include "que_shm.h"

using namespace std;


int main(int argc, char** argv) {
    std::string value="aaa";
    CQueShm que("test_que",1000,10,10);

    string key = argv[1];
    std::string value2;
    int ret = 0;
    if(key == "put")
    {
        ret = que.Put(value);
        printf("ret=%d, v=%s\n", ret, value.c_str());
    }
    else if(key == "pop")
    {
        ret = que.Pop(value2);
        printf("ret=%d, v=%s\n", ret, value2.c_str());
    }

    printf("end\r\n");
    return 0;
}
