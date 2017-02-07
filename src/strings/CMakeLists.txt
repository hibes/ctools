add_subdirectory(splitstring)

include_directories(${CMAKE_INCLUDE_PATH})

add_library(strings_util strings_util.c)
MESSAGE("src/strings/CMakeLists.txt")
