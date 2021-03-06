cmake_minimum_required(VERSION 3.15)
project(c_practice)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(http-parser-demo/src)
include_directories(http-parser-demo/lib)
include_directories(llhttp-demo/src)
include_directories(llhttp-demo/lib)
include_directories(time_consume/src)
include_directories(smart_pointer_perf/src)
include_directories(picohttpparser/lib)
include_directories(libevent_demo/src)
include_directories(libevent_demo/include)
include_directories(que_shm_demo/lib)
include_directories(concurrentqueue_demo/include)
include_directories(thread_entry_demo/src)

add_executable(c_practice
        llhttp-demo/lib/api.c
        llhttp-demo/lib/http.c
        llhttp-demo/lib/llhttp.c
        llhttp-demo/lib/llhttp.h
        llhttp-demo/bench/bench.c
        http-parser-demo/src/main.c
        http-parser-demo/bench/bench.c
        llhttp-demo/src/main.c
        picohttpparser/lib/picohttpparser.c
        picohttpparser/lib/picohttpparser.h
        picohttpparser/bench/bench.c
        time_consume/src/main.cpp
        smart_pointer_perf/src/main.cpp
        thread_local_demo/src/main.cpp
        libevent_demo/src/main.cpp
        que_shm_demo/src/main.cpp
        que_shm_demo/lib/que_shm.cpp
        concurrentqueue_demo/src/main.cpp
        thread_entry_demo/src/main.cpp
        thread_entry_demo/src/registry.h
        mutex_demo/src/main.cpp
        )
