cmake_minimum_required(VERSION 3.15)
project(c_practice C)

set(CMAKE_C_STANDARD 11)

include_directories(llhttp-demo/src)

add_executable(c_practice
        llhttp-demo/src/api.c
        llhttp-demo/src/http.c
        llhttp-demo/src/llhttp.c
        llhttp-demo/src/llhttp.h
        http-parser-demo/src/main.c
        llhttp-demo/src/main.c)
