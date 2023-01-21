cmake_minimum_required(VERSION 3.0)

project(dictionary)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

add_executable(dict
src/main.c
src/dictionary.h
src/dictionary.c
 )