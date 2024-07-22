/**
 * @file analyzer.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Library that calculates current usage for CPU core based on
 * informations from /proc/stat file.
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "utils.h"
#include "globals.h"

/**
 * @brief Function that calculates core usage.
 *
 * @param previdle
 * @param previowait
 * @param idle
 * @param iowait
 * @param prevuser
 * @param prevnice
 * @param prevsystem
 * @param previrq
 * @param prevsoftirq
 * @param prevsteal
 * @param user
 * @param nice
 * @param system
 * @param irq
 * @param softirq
 * @param steal
 * @return double - Result
 */
double calculate_core_usage(unsigned int const previdle, unsigned int const previowait,
                            unsigned int const idle, unsigned int const iowait, unsigned int const prevuser,
                            unsigned int const prevnice, unsigned int const prevsystem,
                            unsigned int const previrq, unsigned int const prevsoftirq,
                            unsigned int const prevsteal, unsigned int const user, unsigned int const nice,
                            unsigned int const system, unsigned int const irq, unsigned int const softirq,
                            unsigned int const steal);

/**
 * @brief Overlay for calculate_core_usage to ease its call
 *
 * @param prev_stats
 * @param curr_stat
 * @return * double - Result
 */
double calculate_core_usage_from_proc_stat_t(proc_stat_t const prev_stats, proc_stat_t const curr_stat);
