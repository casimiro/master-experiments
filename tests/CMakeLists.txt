find_library(LIBPTHREAD NAMES pthread)
find_package(Soci)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include ../ ${SOCI_INCLUDE_DIRS})

link_directories($ENV{GMOCK_HOME}/build $ENV{GMOCK_HOME}/gtest/build)

add_definitions(-std=c++0x)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(tests userprofiletests.cpp main.cpp ../twitteruser.cpp)

enable_testing()

target_link_libraries(tests gmock gtest ${LIBPTHREAD} ${Boost_LIBRARIES} ${SOCI_LIBRARY} ${SOCI_sqlite3_PLUGIN})

add_test(
    NAME tests
    COMMAND tests
)

