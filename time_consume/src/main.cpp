#include <stdio.h>
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include <vector>

#define  MAX_RECORD_NUM     (60*4*60*1000)  // 60 concurrency,4 min,60sec,1000qps

using namespace std;

typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> usType;

struct LatencyRecord {
    uint64_t server_rcv_time;
    uint64_t server_send_time; // envoy rsp to client as server
    uint64_t client_send_time; // envoy send to cluster as client
    uint64_t client_rcv_time;
};
std::vector<LatencyRecord> LatencyRecordArr(MAX_RECORD_NUM);

void test_vector_time() {
    time_t start,end;
    start = clock();

    uint64_t num = 1LL << 30;
    int id = 123456;
    int tmp=0;
    printf("test num %llu\r\n",num);
    // Basically no time consuming
    for (uint64_t i = 0; i < num; ++i) {
        if ( LatencyRecordArr[id].server_rcv_time != 0
             && LatencyRecordArr[id].client_send_time !=0
             && LatencyRecordArr[id].client_rcv_time !=0
             && LatencyRecordArr[id].server_send_time !=0 ) {
            tmp = 1;
        }
    }

    end = clock();
    double total_time = difftime(end,start)/CLOCKS_PER_SEC;
    double parsed_time = total_time/(double)(num);
    printf("take time %f s, every time func take time %f us\r\n",total_time,parsed_time*1000*1000);

}

void test_gettimeofday() {
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

}


void test_string_find() {

}

int main() {

    test_vector_time();
    printf("end\r\n");
    return 0;
}