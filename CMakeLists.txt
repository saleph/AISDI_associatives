cmake_minimum_required(VERSION 2.6)
project(AISDI_Maps)

include(CTest)
enable_testing()

include_directories("${PROJECT_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -Wall -Wextra"
    #-Werror    because of incorrectly written tests - passing int to Key=u_int map => warning
    #-pedantic  because C++ISO forbids anonymous unions and structures - which avoids code redundation
    )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ")

add_subdirectory(src)
add_subdirectory(tests)
