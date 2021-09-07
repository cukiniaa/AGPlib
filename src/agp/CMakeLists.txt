file(GLOB_RECURSE AGP_SOURCES "src/*.cpp")
file(GLOB_RECURSE AGP_HEADERS "include/*.h")

set(AGP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(AGP ${AGP_SOURCES} ${AGP_HEADERS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -W -Wall -pedantic -O0 -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD")
target_include_directories(AGP PUBLIC ${AGP_INCLUDE_DIR})

target_link_libraries(AGP boost_math_c99 boost_thread m CGAL mpfr gmp concert ilocplex cplex pthread dl)