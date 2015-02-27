add_executable(smooth_mpi parallel_main.cpp )
add_library(smooth_library Smooth.cpp SmoothWriter.cpp)
target_link_libraries(smooth_mpi smooth_library  ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES} ${HDF5_LIBRARIES})
