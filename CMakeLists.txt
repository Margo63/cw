cmake_minimum_required(VERSION 3.21)
project(cw C)

set(CMAKE_C_STANDARD 99)

add_executable(cw main.c del_rep.c del_rep.h)
