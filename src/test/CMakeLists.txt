set(PATH_TO_GOOGLETEST lib/googletest/)

#Add the google test subdirectory
add_subdirectory(${PATH_TO_GOOGLETEST})

#include googletest/include dir
include_directories(${PATH_TO_GOOGLETEST}/googletest/include ${PATH_TO_GOOGLETEST}/googletest)

#include the googlemock/include dir
include_directories(${PATH_TO_GOOGLETEST}/googlemock/include)

include_directories(.. ../sorts)

set(TEST_SOURCES ${SOURCES} tests.cpp sorter_harness.h checked_vector.h)


add_executable(runUnitTests gtest.cpp ${TEST_SOURCES})
target_link_libraries(runUnitTests gtest gmock)

