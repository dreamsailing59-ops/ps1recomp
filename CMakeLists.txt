cmake_minimum_required(VERSION 3.10)
project(PS1Recomp)

set(CMAKE_C_STANDARD 11)

add_executable(PS1Recomp boot.c loader.c recompiler.c)
