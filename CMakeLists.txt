cmake_minimum_required(VERSION 3.15)
project(c_practice C)

set(CMAKE_C_STANDARD 11)

include_directories(llhttp-demo/src)
include_directories(llhttp-demo/lib)
include_directories(picohttpparser/lib)

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
        picohttpparser/bench/bench.c)
