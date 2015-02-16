add_executable(smooth main.cpp)
add_library(smooth_library Smooth.cpp)
target_link_libraries(smooth smooth_library)
