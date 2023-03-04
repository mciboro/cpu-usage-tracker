/**
 * @file analyzer.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "analyzer.h"

double calculate_core_usage(unsigned int const previdle, unsigned int const previowait,
                            unsigned int const idle, unsigned int const iowait, unsigned int const prevuser,
                            unsigned int const prevnice, unsigned int const prevsystem,
                            unsigned int const previrq, unsigned int const prevsoftirq,
                            unsigned int const prevsteal, unsigned int const user, unsigned int const nice,
                            unsigned int const system, unsigned int const irq, unsigned int const softirq,
                            unsigned int const steal) {
    unsigned int const PrevIdle = previdle + previowait;
    unsigned int const Idle = idle + iowait;
    unsigned int const PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    unsigned int const NonIdle = user + nice + system + irq + softirq + steal;
    unsigned int const PrevTotal = PrevIdle + PrevNonIdle;
    unsigned int const Total = Idle + NonIdle;

    double const totald = Total - PrevTotal;
    double const idled = Idle - PrevIdle;

    return (totald - idled) / totald * 100;
}

double calculate_core_usage_from_proc_stat_t(proc_stat_t const prev_stats, proc_stat_t const curr_stat) {
    return calculate_core_usage(prev_stats.idle, prev_stats.iowait, curr_stat.idle, curr_stat.iowait,
                                prev_stats.user, prev_stats.nice, prev_stats.system, prev_stats.irq,
                                prev_stats.softirq, prev_stats.steal, curr_stat.user, curr_stat.nice,
                                curr_stat.system, curr_stat.irq, curr_stat.softirq, curr_stat.steal);
}