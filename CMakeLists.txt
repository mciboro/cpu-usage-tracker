cmake_minimum_required(VERSION 3.0.0)
project(cpu-usage-tracker VERSION 0.1.0)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(reader SHARED reader/reader.c)
target_include_directories(reader PUBLIC ./)

add_executable(reader-test reader/reader_test.c)
target_link_libraries(reader-test reader ringbuffer)
target_include_directories(reader-test PUBLIC ./)

add_library(ringbuffer SHARED ring_buffer/ring_buffer.c)
target_include_directories(ringbuffer PUBLIC ./)

add_executable(ringbuffer-test ring_buffer/ring_buffer_test.c)
target_link_libraries(ringbuffer-test ringbuffer)

add_library(analyzer SHARED analyzer/analyzer.c)
target_include_directories(analyzer PUBLIC ./)

add_executable(analyzer-test analyzer/analyzer_test.c)
target_link_libraries(analyzer-test analyzer)

add_executable(cut main.c)
target_link_libraries(cut analyzer ringbuffer reader pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
