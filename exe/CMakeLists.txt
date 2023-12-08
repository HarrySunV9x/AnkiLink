# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

SET(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
SET(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

# 设置你的项目名称
project(AnkiLink)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加你的源文件
add_executable(AnkiLink Main.cpp AnkiSocket.cpp)

# 链接库
target_link_libraries(AnkiLink wsock32)

