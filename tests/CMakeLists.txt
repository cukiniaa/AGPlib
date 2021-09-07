
# add_executable(experiment main.cpp.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -W -Wall -pedantic  -O0 -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -lstdc++fs")

# target_link_libraries(experiment AGP)

set(TEST_DIR .)

add_library(Catch INTERFACE)

set(CATCH_INCLUDE_DIR /usr/include/catch2)

target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES ${TEST_DIR}/001-arr_partition_test.cpp ${TEST_DIR}/001-arr_utils_test.cpp ${TEST_DIR}/001-arr_visibility_test.cpp)

add_executable(agp_test ${TEST_SOURCES})
target_link_libraries(agp_test Catch AGP)

enable_testing()
add_test(NAME TestBase COMMAND agp_test)

