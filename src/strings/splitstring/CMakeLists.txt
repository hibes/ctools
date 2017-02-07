set(SPLITSTRING_BIN splitstring)

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(${SPLITSTRING_BIN} main.c)

set(LIBS ${LIBS} strings_util)

target_link_libraries(${SPLITSTRING_BIN} ${LIBS})
