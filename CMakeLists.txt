cmake_minimum_required(VERSION 3.26)
project(untitled4 C)

set(CMAKE_C_STANDARD 11)

set(PROJECT_HEADERS
        main.h
)
set(PROJECT_SOURCES
        Menu.c
        main.c
)

add_executable(Menu ${PROJECT_SOURCES} ${PROJECT_HEADERS})

