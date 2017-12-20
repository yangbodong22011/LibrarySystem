cmake_minimum_required(VERSION 2.8)
project(bookManage)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-std=c99 -lncursesw -lpthread")

set(SRC_LIST src/bookManage.c src/file.c src/menu.c src/win.c)
add_executable(bookManage ${SRC_LIST})