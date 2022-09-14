cmake_minimum_required(VERSION 3.9)
project(mergesorts)

set(CMAKE_CXX_STANDARD 17)
# Use -O3 for release builds
#string(REPLACE "-O2" "-Ofast -march=native -mtune=native" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
#string(REPLACE "-O2" "-Ofast" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W -Wall -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native -mtune=native")

if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected!")
endif()

add_subdirectory(src)



