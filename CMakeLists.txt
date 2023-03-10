cmake_minimum_required(VERSION 3.0.0)
project(cpu-usage-tracker VERSION 0.1.0)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(reader SHARED src/reader/reader.c)
target_include_directories(reader PUBLIC src/)

add_executable(reader-test src/reader/reader_test.c)
target_link_libraries(reader-test reader ringbuffer)
target_include_directories(reader-test PUBLIC src/)

add_library(ringbuffer SHARED src/ring_buffer/ring_buffer.c)
target_include_directories(ringbuffer PUBLIC src/)

add_executable(ringbuffer-test src/ring_buffer/ring_buffer_test.c)
target_link_libraries(ringbuffer-test ringbuffer)

add_library(analyzer SHARED src/analyzer/analyzer.c)
target_include_directories(analyzer PUBLIC src/)

add_executable(analyzer-test src/analyzer/analyzer_test.c)
target_link_libraries(analyzer-test analyzer)

add_executable(cut src/cut.c)
target_link_libraries(cut analyzer ringbuffer reader pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
