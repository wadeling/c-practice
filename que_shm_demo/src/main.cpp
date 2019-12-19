#include <stdio.h>
#include <thread>
#include "que_shm.h"

using namespace std;

void que_write() {
    std::string value="aaa";
    CQueShm que("test_que",10000,1000,10);
    for (int i = 0; i < 100; ++i) {
        int ret = que.Put(value);
        printf("que put ret %d \r\n",ret);
    }
    printf("que write end\r\n");
}

void que_read() {
    std::string value;
    CQueShm que("test_que",10000,1000,10);
    for (int i = 0; i < 100; ++i) {
        int ret = que.Pop(value);
        printf("que read value %s,ret %d \r\n",value.c_str(),ret);
    }
    printf("que read end");
}

int test_multi_thread_rw() {
    std::thread t1([]{
        que_write();
    });
    std::thread t2([]{
        que_read();
    });
    t1.join();
    t2.join();
    return 0;
}

int test_coredump_case() {
    CQueShm que("test_shm", 10, 10, 10);

    std::string value;
    int ret;

    if( (ret = que.Put("value")) != 0) {
        printf("put err %d\r\n",ret);
        return -1;
    }

    if( (ret = que.Pop(value)) != 0) {
        printf("pop err %d \r\n",ret);
        return -1;
    }
}

void test_base_rw(std::string key) {
    int result;
    std::string value="aaa";
    CQueShm que("test_que",1000,10,10);

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
    printf("pop %s\r\n",value2.c_str());
}

int main() {
    test_multi_thread_rw();
    printf("end\r\n");
    return 0;
}
