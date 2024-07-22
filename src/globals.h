/**
 * @file globals.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief File containing global variables
 * @version 0.1
 * @date 2024-07-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "ring_buffer/ring_buffer.h"

#define WATCHDOG_THREADS_NUM 3

ring_buffer_t *stats_buf = NULL, *results_buf = NULL, *watchdog_buf = NULL, *logger_buf = NULL;
struct timespec hb_status_arr[WATCHDOG_THREADS_NUM] = {0};
