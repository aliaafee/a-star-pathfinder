cmake_minimum_required(VERSION 3.1.0)
project(pathfinder)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory("pybind11")

include_directories("include")
pybind11_add_module(pathfinder "./src/main.cpp" "./src/Pathfinder.cpp")
