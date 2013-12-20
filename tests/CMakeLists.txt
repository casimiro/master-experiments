find_library(LIBPTHREAD NAMES pthread)

message(WARNING $ENV{GMOCK_HOME}/build)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include ../)
link_directories($ENV{GMOCK_HOME}/build $ENV{GMOCK_HOME}/gtest/build)

add_definitions(-std=c++0x)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(tests userprofiletests.cpp main.cpp)

enable_testing()

target_link_libraries(tests gmock gtest ${LIBPTHREAD} ${Boost_LIBRARIES})

add_test(
    NAME tests
    COMMAND tests
)

