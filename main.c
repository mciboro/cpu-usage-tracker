/**
 * @file main.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Automatic test
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "analyzer/analyzer.h"
#include "reader/reader.h"
#include "ring_buffer/ring_buffer.h"

ring_buffer_t *stats = NULL, *results = NULL;
proc_stat_t buf[4][2] = {0};

void *reader_func() {
    for (;;) {
        read_proc_stat(stats, "/proc/stat");
        sleep(1);
    }
    return NULL;
}

void *analyzer_func() {
    for (;;) {
        if (stats->write_index < stats->read_index)
            stats->read_index = 0;

        while (stats->read_index < stats->write_index) {
            data_t tmp = {0};
            ringbuffer_get(stats, &tmp, STAT);
            buf[tmp.stat.core_number][0] = buf[tmp.stat.core_number][1];
            buf[tmp.stat.core_number][1] = tmp.stat;
        }

        for (int i = 0; i < 4; i++) {
            double const result = calculate_core_usage_from_proc_stat_t(buf[i][0], buf[i][1]);
            data_t result_data = {0};
            result_data.result.core_number = i;
            result_data.result.result = result;
            ringbuffer_add(results, result_data, RESULT);
        }
        sleep(1);
    }
    return NULL;
}

void *printer_func() {
    for (;;) {
        if (results->write_index < results->read_index)
            results->read_index = 0;

        while (results->read_index < results->write_index) {
            data_t tmp = {0};
            ringbuffer_get(results, &tmp, STAT);
            printf("Core %d: %.2f%%\n", tmp.result.core_number, tmp.result.result);
        }

        printf("\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    ringbuffer_create(&stats, RING_BUFFER_SIZE);
    ringbuffer_create(&results, RING_BUFFER_SIZE);

    pthread_t reader, analyzer, printer;
    pthread_create(&reader, NULL, reader_func, NULL);
    pthread_create(&analyzer, NULL, analyzer_func, NULL);
    pthread_create(&printer, NULL, printer_func, NULL);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(reader, NULL);

    ringbuffer_destroy(&stats);
    ringbuffer_destroy(&results);
}