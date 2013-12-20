project(master-experiments)
cmake_minimum_required(VERSION 2.8)

add_executable(master-experiments main.cpp)

install(TARGETS master-experiments RUNTIME DESTINATION bin)

add_subdirectory(tests)