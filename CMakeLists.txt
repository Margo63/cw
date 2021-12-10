cmake_minimum_required(VERSION 3.21)
project(cw C)

set(CMAKE_C_STANDARD 99)

add_executable(cw main.c text_processing.c text_processing.h)
