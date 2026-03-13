cmake_minimum_required(VERSION 3.12)
project("PS1Recomp" VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# Tell the compiler to look in 'src' for headers
include_directories(src)

# Define the executable using the files in the src directory
add_executable(ps1recomp 
    src/boot.c 
    src/loader.c 
    src/recompiler.c
)