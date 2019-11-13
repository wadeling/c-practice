#include <stdio.h>
#include <chrono>
#include <time.h>
#include <sys/time.h>

using namespace std;

typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> usType;

int main() {
    time_t start,end;
    struct timeval start2;
    int err;
    start = clock();

    uint64_t num = 1LL << 30;
    printf("test num %llu\r\n",num);
    for (uint64_t i = 0; i < num; ++i) {

        err = gettimeofday(&start2, NULL);
        if (err != 0) {
            printf("get time of day err.\r\n");
            break;
        }
//        usType tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
//        tp.time_since_epoch().count();
    }

    end = clock();
    double total_time = difftime(end,start)/CLOCKS_PER_SEC;
    double parsed_time = total_time/(double)(num);
    printf("take time %f s, every time func take time %f us\r\n",total_time,parsed_time*1000*1000);

    printf("end\r\n");
    return 0;
}