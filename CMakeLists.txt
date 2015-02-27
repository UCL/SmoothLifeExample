cmake_minimum_required(VERSION 2.8)
project(SmoothLifeTeachingExample)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(MPI)
find_package(HDF5)
include_directories(${MPI_CXX_INCLUDE_PATH} ${HDF5_INCLUDE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_C_COMPILE_FLAGS}")

include_directories(src)
include_directories(lib)

add_subdirectory(src)
add_subdirectory(test)

