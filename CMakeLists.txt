cmake_minimum_required(VERSION 3.0.0)
project(cpu-usage-tracker VERSION 0.1.0)

add_library(procstat SHARED proc_stat/proc_stat.c)

# add_executable(procstat-test proc_stat/proc_stat_test.c proc_stat/proc_stat.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
