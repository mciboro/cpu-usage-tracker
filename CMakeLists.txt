cmake_minimum_required(VERSION 3.0.0)
project(cpu-usage-tracker VERSION 0.1.0)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(reader SHARED reader/reader.c)
target_include_directories(reader PUBLIC ./)

add_executable(reader-test reader/reader_test.c)
target_link_libraries(reader-test reader ringbuffer)
target_include_directories(reader-test PUBLIC ./)

add_library(ringbuffer SHARED ring_buffer/ring_buffer.c)

add_executable(ringbuffer-test ring_buffer/ring_buffer_test.c)
target_link_libraries(ringbuffer-test ringbuffer)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
