#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>
#include "registry.h"

std::timed_mutex coutMutex;

class Animal {
public:
    virtual void talk() = 0;
};

class Dog: public Animal {
public:
    void talk() override {
        while (true) {
            printf("i am dog \r\n");
            sleep(2);
        }
    }
};

class People {
public:
    virtual void talk() =0;
};

class Student: public People {
public:
    void talk() override ;
};
void Student::talk() {
//    while (true) {
    for (int i = 0; i < 10; ++i) {
//        std::lock_guard<std::mutex> guard(coutMutex);
        if (coutMutex.try_lock_for(std::chrono::seconds(5))) {
            std::cout << "i am student " << i << std::endl;
            coutMutex.unlock();
        } else {
            std::cout << "student lock faled" << std::endl;
        }
        sleep(2);
    }
    std::cout << "student talk end" << std::endl;
}

class Teacher: public People {
public:
    void talk() override ;
};
void Teacher::talk() {
//    while (true) {
    for (int i = 0; i < 10; ++i) {
//        std::lock_guard<std::mutex> guard(coutMutex);
        if (coutMutex.try_lock_for(std::chrono::seconds(5))) {
            std::cout << "i am teacher " << i << std::endl;
            coutMutex.unlock();
        } else {
            std::cout << "student lock faled" << std::endl;
        }
        sleep(2);
    }
    std::cout << "teacher talk end " << std::endl;
}

void test_registry() {
    Student s;
    Teacher t;
    Dog d;
    FactoryRegistry<Animal>::registerFactory(d,"dog");
    FactoryRegistry<People>::registerFactory(s,"student");
    FactoryRegistry<People>::registerFactory(t,"teacher");
    for (const auto& factory : FactoryRegistry<People>::factories()) {
        printf("factory name %s\r\n",factory.first.c_str());
        People* p = dynamic_cast<People*>(factory.second);
        printf("base ptr %p\r\n",p);
//        if (p) {
//            p->talk();
//        }
    }
}

int main(){
    test_registry();
    return 1;

    std::cout << std::endl;

//    Student s;
//    std::thread t1([&s]() {
//        s.talk();
//    });
//
//    Teacher t;
//    std::thread t2([&t]() {
//        t.talk();
//    });
//    t1.join();
//    t2.join();

    std::vector<People*> v;
    Student s;
    Teacher t;
    v.push_back(&s);
    v.push_back(&t);

    std::thread people_thread[2];
    for (int i = 0; i < int(v.size()); ++i) {
//        People *ret = dynamic_cast<People*>(v[i]);
        People *p=v[i];
//        const char* cname = typeid(*p).name();
//        printf("cname %s \r\n",cname);
        people_thread[i] = std::thread([p]() {
            p->talk();
        });
    }

    for (int j = 0; j < 2; ++j) {
        people_thread[j].join();
    }
}