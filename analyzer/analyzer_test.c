/**
 * @file analyzer_test.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Tests of analyzer library.
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "analyzer.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    proc_stat_t curr_stat = {0, 534982, 864, 110714, 8074729, 15548, 0, 1189, 0, 0, 0};
    proc_stat_t prev_stat = {0, 535081, 864, 110753, 8076755, 15551, 0, 1190, 0, 0, 0};
    double const result = calculate_core_usage_from_proc_stat_t(curr_stat, prev_stat);
    assert(result > 6.4 && result < 6.5);

    proc_stat_t zero_stat = {0};
    calculate_core_usage_from_proc_stat_t(zero_stat, curr_stat);
    calculate_core_usage_from_proc_stat_t(prev_stat, zero_stat);
    calculate_core_usage_from_proc_stat_t(zero_stat, zero_stat);
}