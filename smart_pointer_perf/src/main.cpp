#include <stdio.h>
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <memory>

using namespace std;
enum SmartPointerType {
    NewType,
    SharedType,
    MakeSharedType,
    UniqueType,
    MakeUniqueType,
};

void smart_pointer_perf(SmartPointerType type,std::string test_name) {
    uint64_t numInt = 1LL << 30;
    time_t start,end;
    start = clock();

    for ( long long i=0 ; i < numInt; ++i){
        if (type == NewType) {
            // fast: allmost 0 ns
            int* tmp(new int(i));
            delete tmp;
        } else if (type == SharedType) {
            // most slow: 167 ns
            std::shared_ptr<int> tmp(new int(i));
        } else if ( type == MakeSharedType) {
            // normal slow:75ns
            std::shared_ptr<int> tmp(std::make_shared<int>(i));
        } else if (type == UniqueType) {
            // fast: allmost 0 ns
            std::unique_ptr<int> tmp(new int(i));
        } else if (type == MakeUniqueType) {
            // fast: allmost 0 ns
             std::unique_ptr<int> tmp(std::make_unique<int>(i));
        }
    }

    end = clock();
    double total_time = difftime(end,start)/CLOCKS_PER_SEC;
    double parsed_time = total_time/ static_cast<double>(numInt);
    printf("[%s] take time %f s, count %llu,smart pointer take time %f us\r\n",test_name.c_str(),total_time,numInt,parsed_time*1000*1000);
}

std::unique_ptr<int> foo()
{
    std::unique_ptr<int> p( new int(10) );

    return p;                   // 1
}
std::unique_ptr<int> foo2() {
    return foo();
}
void test_return_unique_ptr() {
    std::unique_ptr<int> p = foo2();
    printf("p value %d \r\n",*p);
}

int main() {

//    smart_pointer_perf(NewType,"test new");
//    smart_pointer_perf(SharedType,"test shared_ptr");
//    smart_pointer_perf(MakeSharedType,"test make_shared");
//    smart_pointer_perf(UniqueType,"test unique_ptr");
//    smart_pointer_perf(MakeUniqueType,"test make_unique");

    test_return_unique_ptr();

    printf("end\r\n");
    return 0;
}