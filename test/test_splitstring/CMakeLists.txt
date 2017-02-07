set(TEST_SPLITSTRING_BIN test_splitstring)

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(${TEST_SPLITSTRING_BIN} main.c)

set(LIBS ${LIBS} strings_util)

target_link_libraries(${TEST_SPLITSTRING_BIN} ${LIBS})
