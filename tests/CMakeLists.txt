find_library(LIBPTHREAD NAMES pthread)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include ../ ${SOCI_INCLUDE_DIRS})

link_directories($ENV{GMOCK_HOME}/build $ENV{GMOCK_HOME}/gtest/build)

add_executable(tests userprofiletests.cpp main.cpp ../twitteruser.cpp)

enable_testing()

target_link_libraries(tests gmock gtest ${LIBPTHREAD} ${Boost_LIBRARIES} Qt5::Sql Qt5::Core)

add_test(
    NAME tests
    COMMAND tests
)

add_custom_command(TARGET tests COMMAND cp ${CMAKE_SOURCE_DIR}/tests/sql/setup.sql ${CMAKE_BINARY_DIR}/tests)
add_custom_command(TARGET tests COMMAND cp ${CMAKE_SOURCE_DIR}/tests/sql/teardown.sql ${CMAKE_BINARY_DIR}/tests)