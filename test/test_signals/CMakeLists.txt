set(TEST_SIGNALS_BIN test_signals)

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(${TEST_SIGNALS_BIN} main.c)

set(LIBS ${LIBS} signals_util)

target_link_libraries(${TEST_SIGNALS_BIN} ${LIBS})
