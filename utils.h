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

/**
 * @brief Structure holding statistics extracted
 * from /proc/stat file for one cpu core.
 *
 */
typedef struct proc_stat_t {
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
typedef struct core_result_t {
    uint32_t core_number;
    double result;
} core_result_t;
