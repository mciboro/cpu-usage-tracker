/**
 * @file utils.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file containing data structures and functions
 * which help read and extract informations from /proc/stat file.
 * @version 0.1
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/**
 * @brief For suppresing unused parameters 
 * 
 */
#define UNUSED(x) (void)(x)

/**
 * @brief Return values
 * 
 */
typedef enum { SUCCESS, VOID_ARG, INV_ARG, FILE_ERROR, MEMORY_ERROR, RET_ERROR, WAIT_TIMEOUT } ReturnType_t;
typedef enum { READER, ANALYZER, PRINTER, LOGGER, WATCHDOG } ThreadType_t;

const char *ThreadNames[] = {"Reader", "Analyzer", "Printer", "Logger", "Watchdog"};

/**
 * @brief Structure holding statistics extracted
 * from /proc/stat file for one cpu core.
 *
 */
typedef struct {
    uint32_t core_number;
    uint32_t user;
    uint32_t nice;
    uint32_t system;
    uint32_t idle;
    uint32_t iowait;
    uint32_t irq;
    uint32_t softirq;
    uint32_t steal;
} proc_stat_t;

/**
 * @brief Structure holding current procent of
 * core usage and number of a core.
 *
 */
typedef struct {
    uint32_t core_number;
    double result;
} core_result_t;

/**
 * @brief Structure of log which is written to  
 * log file.
 *
 */
typedef struct {
    time_t timestamp;
    char *text;
} log_t;

/**
 * @brief Heartbeat structure which is used by
 * threads to report to watchdod their activity 
 */
typedef struct {
    struct timespec timestamp;
    ThreadType_t thread_type;
} heartbeat_t;