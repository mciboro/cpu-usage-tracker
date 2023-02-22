/**
 * @file proc_stat.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file containing data structures and functions
 * which help read and extract informations from /proc/stat file.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ring_buffer/ring_buffer.h"

#define MAX_LINE_SIZE 256 // Max size of one line read from file

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
    uint32_t guest;
    uint32_t guest_nice;
} proc_stat_t;

/**
 * @brief Function that reads /proc/stat and puts
 * formatted data into ring buffer.
 *
 * @param rbuf - Pointer to ring buffer
 * @param proc_stat_dir - Directory of proc stat file
 * @return unsigned int
 */
unsigned int read_proc_stat(ring_buffer_t *const rbuf, char const *const proc_stat_dir);