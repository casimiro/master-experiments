project(master-experiments)
cmake_minimum_required(VERSION 2.8)

find_package(Boost COMPONENTS regex date_time REQUIRED)
find_package(Soci)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(master-experiments twitteruser.cpp main.cpp)

target_link_libraries(master-experiments soci_core ${Boost_LIBRARIES})

install(TARGETS master-experiments RUNTIME DESTINATION bin)

add_subdirectory(tests)