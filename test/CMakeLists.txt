add_executable(catch catch.cpp)
target_link_libraries(catch smooth_library  ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
