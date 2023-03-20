/**
 * @file cut.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file holding utils needed by CUT program.
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "analyzer/analyzer.h"
#include "logger/logger.h"
#include "reader/reader.h"

/**
 * @brief Struct holding previous and current informations
 * from proc stat file.
 *
 */
typedef struct {
    proc_stat_t prev;
    proc_stat_t curr;
} stat_packet_t;

/**
 * @brief Arguments of reader function.
 *
 */
typedef struct {
    ring_buffer_t *stats_buf;
} reader_args_t;

/**
 * @brief Arguments of analyzer function.
 *
 */
typedef struct {
    ring_buffer_t *stats_buf;
    ring_buffer_t *results_buf;
    stat_packet_t *stat_packets;
    unsigned int *core_number;
} analyzer_args_t;

/**
 * @brief Arguments of printer function.
 *
 */
typedef struct {
    ring_buffer_t *results_buf;
    double *curr_results;
    unsigned int *core_number;
} printer_args_t;

/**
 * @brief Arguments of logger function.
 *
 */
typedef struct {
    char *log_filename;
} logger_args_t;