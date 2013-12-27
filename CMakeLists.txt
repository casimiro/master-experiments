project(master-experiments)
cmake_minimum_required(VERSION 2.8.9)

find_package(Boost COMPONENTS regex date_time REQUIRED)
find_package(Qt5Core)
find_package(Qt5Sql)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-std=c++11 -std=gnu++11 -pthread -W -Wall -Wno-unused-function)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE ON)
else()
    add_definitions(-Ofast)
endif()

include_directories(${Boost_INCLUDE_DIRS})

add_executable(master-experiments evaluation.cpp tweet.cpp twitteruser.cpp main.cpp)

target_link_libraries(master-experiments Qt5::Sql Qt5::Core ${Boost_LIBRARIES})

install(TARGETS master-experiments RUNTIME DESTINATION bin)

add_custom_command(TARGET master-experiments COMMAND cp -r ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR})

add_subdirectory(tests)