cmake_minimum_required(VERSION 3.15)
project(parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(parser src/main.cpp src/scanner.cpp src/scanner.h src/parser.cpp src/parser.h)