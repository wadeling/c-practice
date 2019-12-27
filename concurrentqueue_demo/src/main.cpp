#include <stdio.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <unistd.h>

#include "concurrentqueue.h"
#include "blockingconcurrentqueue.h"

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

void test_block_que() {
    moodycamel::BlockingConcurrentQueue<int> q;
    std::thread producer([&]() {
        for (int i = 0; i != 100; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(i % 10));
            q.enqueue(i);
        }
    });

    std::thread consumer([&]() {
        for (int i = 0; i != 100; ++i) {
            int item;
            printf("before wait \r\n");
            q.wait_dequeue(item);
            printf("wait dequ %d\r\n",item);
        }
    });

    producer.join();
    consumer.join();

    assert(q.size_approx() == 0);
}

void test_multi_thread_block() {
    moodycamel::BlockingConcurrentQueue<int> q;
    const int ProducerCount = 8;
    const int ConsumerCount = 8;
    int dequeued[100] = { 0 };
    std::thread producers[ProducerCount];
    std::thread consumers[ConsumerCount];
    std::atomic<int> promisedElementsRemaining(ProducerCount * 10);
    for (int i = 0; i != ProducerCount; ++i) {
        producers[i] = std::thread([&](int i) {
            for (int j = 0; j != 10; ++j) {
//                std::this_thread::sleep_for(std::chrono::seconds( 1));
                int item = i*10+j;
                q.enqueue(item);
                std::cout << "producer thread " << std::this_thread::get_id() << " enqueue " << item << std::endl;
            }
            std::cout << "producer thread " << std::this_thread::get_id() << " end " << std::endl;
        },i);
    }
    std::cout << "all producer end" << std::endl;

    for (int i = 0; i != ConsumerCount; ++i) {
        consumers[i] = std::thread([&]() {
            int item;
            while (promisedElementsRemaining.fetch_sub(1, std::memory_order_relaxed) > 0) {
                bool ret = q.wait_dequeue_timed(item,std::chrono::seconds(5));
                if (ret) {
                    std::cout << "consumer thread " << std::this_thread::get_id() << " dequeue " << item << std::endl;
                    ++dequeued[item];
                } else {
                    std::cout << "consumer thread " << std::this_thread::get_id() << " timeout " << std::endl;
                    int ret = promisedElementsRemaining.load(std::memory_order_relaxed);
                    std::cout << "remain count " << ret << std::endl;
                    if (ret == 0) {
                        break;
                    }
                }
            }
            std::cout << "consumer thread " << std::this_thread::get_id() << " end " << std::endl;
        });
    }

    std::cout << "all consumer end " << std::endl;

    for (int i = 0; i != ProducerCount; ++i) {
        producers[i].join();
    }
    for (int i = 0; i != ConsumerCount; ++i) {
        consumers[i].join();
    }

    std::cout << "result: " << endl;
    for (int k = 0; k < sizeof(dequeued)/ sizeof(dequeued[0]); ++k) {
       std::cout << "index " << k << " value " << dequeued[k] << endl;
    }

    int ret = promisedElementsRemaining.load(std::memory_order_relaxed);
    std::cout << "ret " << ret << std::endl;
}

void test_signal() {
    moodycamel::BlockingConcurrentQueue<int> q;
    std::thread consumer[5];
    for (int i = 0; i < 5; ++i) {
        consumer[i] = std::thread([&](int i) {
            while (true) {
                printf("thread[%d] start wait \r\n",i);
                int item;
                q.wait_dequeue(item);
                printf("thread[%d] dequeue %d \r\n",i,item);
            }
        },i);
    }

    sleep(5);

    std::thread producer = std::thread([&]() {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 2; ++j) {
                bool ret = q.enqueue(i);
                printf("producer enqueue %d,ret %d\r\n",i,ret);
            }
           sleep(2);
        }
    });

    for (int j = 0; j < 5; ++j) {
        consumer[j].join();
    }
    producer.join();

}

struct student {
    student(int age) : age_(age) {}
    int age_;
} ;
void test_unique_ptr_enqueue() {
    student s(11);
    student* stu_p = &s;

    moodycamel::ConcurrentQueue<std::unique_ptr<student>> q;
    std::unique_ptr<student> p = std::make_unique<student>(10);
//    std::unique_ptr<student> p(new student(10)); //ok,new 变量放在heap，由程序管理，另一个线程结束后释放
//    std::unique_ptr<student> p(stu_p); // coredump. 局部变量放在stack，由系统管理释放。而另一个线程结束时也会释放这块内存。
//    std::unique_ptr<student> p(&s); //coredump
    /*
    student* pxx = new student(10); //ok
    std::unique_ptr<student> p(pxx);
    */

    std::thread producer = std::thread([&]() {
        //std::move, 原来的对象销毁
        bool ret = q.enqueue(std::move(p));
        printf("producer enqueue ,ret %d\r\n",ret);
    });

    sleep(10);

    printf("student unique ptr %p\r\n",p.get());

    std::thread consumer = std::thread([&]() {
        std::unique_ptr<student> p;
        bool ret = q.try_dequeue(p);
        printf("producer dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });

    producer.join();
    consumer.join();
}

void test_multi_unique_ptr_enqueue() {
    moodycamel::ConcurrentQueue<std::unique_ptr<student>> q;
//    std::unique_ptr<student> p = std::make_unique<student>(10);
//    std::unique_ptr<student> p1(p.get()); // 除了取到错误数据，还会coredump

    student* pxx = new student(10);
    std::unique_ptr<student> p(pxx);
    std::unique_ptr<student> p1(pxx);

    //两个unique ptr.get() 相同，即指向相同地址。
    printf("init student unique ptr %p,ptr1 %p,ptr age %d,p1 age %d\r\n",p.get(),p1.get(),p->age_,p1->age_);

    std::thread producer = std::thread([&]() {
        //std::move, 原来的对象销毁
        bool ret = q.enqueue(std::move(p));
        printf("producer enqueue ,ret %d\r\n",ret);
        bool ret1 = q.enqueue(std::move(p1));
        printf("producer enqueue ,ret1 %d\r\n",ret1);
    });

    sleep(3);

    printf("student unique ptr %p,ptr1 %p\r\n",p.get(),p1.get());

    std::thread consumer = std::thread([&]() {
        std::unique_ptr<student> p;
        bool ret = q.try_dequeue(p);
        printf("consumer dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });
    sleep(1);

    std::thread consumer1 = std::thread([&]() {
        std::unique_ptr<student> p;
        bool ret = q.try_dequeue(p);
        // age 是一个错误的值。第一个线程处理完后释放了对应的内存，所以这里得到的是一个错误的值
        printf("consumer1 dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });

    producer.join();
    consumer.join();
    consumer1.join();
}

void test_multi_shared_ptr_enqueue() {
    moodycamel::ConcurrentQueue<std::shared_ptr<student>> q;
    std::shared_ptr<student> p = std::make_shared<student>(10);
    std::shared_ptr<student> p1(p.get());//两个shared_ptr指向相同对象，会double free

    //shared ptr.get() 相同，即指向相同地址。
    printf("init student unique ptr %p,ptr1 %p,ptr age %d,p1 age %d\r\n",p.get(),p1.get(),p->age_,p1->age_);

    std::thread producer = std::thread([&]() {
        //shared ptr ，不用move
        bool ret = q.enqueue(p);
        printf("producer enqueue ,ret %d\r\n",ret);
        bool ret1 = q.enqueue(p1);
        printf("producer enqueue ,ret1 %d\r\n",ret1);
    });

    sleep(3);

    printf("student shared ptr %p,ptr1 %p\r\n",p.get(),p1.get());

    std::thread consumer = std::thread([&]() {
        std::shared_ptr<student> p;
        bool ret = q.try_dequeue(p);
        printf("consumer dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });
    sleep(1);

    std::thread consumer1 = std::thread([&]() {
        std::shared_ptr<student> p;
        bool ret = q.try_dequeue(p);
        // age 是一个错误的值。第一个线程处理完后释放了对应的内存，所以这里得到的是一个错误的值
        printf("consumer1 dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });

    producer.join();
    consumer.join();
    consumer1.join();
}

void test_shared_ptr_enqueue() {
    student s(10);
    moodycamel::ConcurrentQueue<std::shared_ptr<student>> q;
    std::shared_ptr<student> p = std::make_shared<student>(10);

    //不能使用局部变量，shared_ptr超出scope的时候会尝试delete变量，而程序不能删除一个stack变量，stack变量由系统处理。
//    std::shared_ptr<student> p(&s);

    //这样ok。shared_ptr销毁时不做任何delete动作。但是不推荐，失去了shared_ptr的意义了.
//    std::shared_ptr<student> p(&s,[](student*) {});

    printf("init student unique ptr %p,ptr age %d\r\n",p.get(),p->age_);

    std::thread producer = std::thread([&]() {
        //shared ptr ，不用move
        bool ret = q.enqueue(p);
        printf("producer enqueue ,ret %d\r\n",ret);
    });

    sleep(3);

    printf("student shared ptr %p\r\n",p.get());

    std::thread consumer = std::thread([&]() {
        std::shared_ptr<student> p;
        bool ret = q.try_dequeue(p);
        printf("consumer dequeue ,ret %d,%p,age %d\r\n",ret,p.get(),p->age_);
    });

    producer.join();
    consumer.join();
}

void test_local_str_enqueue() {
    moodycamel::ConcurrentQueue<std::string> q;
    std::string s("aaaa");

    std::thread producer = std::thread([&]() {
        bool ret = q.enqueue(std::move(s));
        printf("producer enqueue ,ret %d\r\n",ret);
    });
    sleep(3);
    printf("after enqueue string %s\r\n",s.data());

    std::thread consumer = std::thread([&]() {
        std::string s;
        bool ret = q.try_dequeue(s);
        printf("consumer dequeue ,ret %d,%s\r\n",ret,s.data());
    });

    producer.join();
    consumer.join();
}

class Teacher {
public:
    std::string name_;
    int age_;
};
class People {
public:
    Teacher teacher_;
};
using TeacherSharedPtr = std::shared_ptr<Teacher> ;
void test_member_share_ptr_enqueue() {
    moodycamel::ConcurrentQueue<TeacherSharedPtr> q;
    std::thread producer = std::thread([&]() {
        std::unique_ptr<People> people_ptr = std::make_unique<People>();
        people_ptr->teacher_.age_=10;
        people_ptr->teacher_.name_ = "aaa";
        // coredump,两个指针管理一个对象
        TeacherSharedPtr teacher_ptr(&people_ptr->teacher_);
        printf("teacher age %d name %s\r\n",teacher_ptr->age_,teacher_ptr->name_.c_str());

        bool ret = q.enqueue(teacher_ptr);
        printf("producer enqueue ,ret %d\r\n",ret);
    });
    sleep(3);
    printf("after enqueue \r\n");

    std::thread consumer = std::thread([&]() {
        TeacherSharedPtr teacher_ptr;
        bool ret = q.try_dequeue(teacher_ptr);
        printf("consumer dequeue ,ret %d,%d\r\n",ret,teacher_ptr->age_);
    });

    producer.join();
    consumer.join();
}

int main() {
    //test_base_rw();

//    test_multi_thread_rw();

//    test_multi_thread_block();
//    test_block_que();
//    test_signal();
//    test_unique_ptr_enqueue();
//    test_multi_unique_ptr_enqueue();
//    test_multi_shared_ptr_enqueue();
//    test_shared_ptr_enqueue();

//    test_local_str_enqueue();
    test_member_share_ptr_enqueue();

    std::cout << "done!\n";
    return 0;
}
