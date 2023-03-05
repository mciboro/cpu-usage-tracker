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
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#include "analyzer/analyzer.h"
#include "reader/reader.h"
#include "ring_buffer/ring_buffer.h"

typedef struct {
    proc_stat_t prev;
    proc_stat_t curr;
} stat_packet_t;

unsigned int core_number = 0;

ring_buffer_t *stats = NULL, *results = NULL;
sem_t stats_mutex, results_mutex;
stat_packet_t *buf;

void *reader_func() {
    for (;;) {
        sem_wait(&stats_mutex);
        read_proc_stat(stats, "/proc/stat");
        sem_post(&stats_mutex);
        sleep(1);
    }
    return NULL;
}

void *analyzer_func() {
    for (;;) {
        sem_wait(&stats_mutex);
        if (stats->write_index < stats->read_index)
            stats->read_index = 0;

        while (stats->read_index < stats->write_index) {
            data_t tmp = {0};
            ringbuffer_get(stats, &tmp);
            buf[tmp.stat.core_number].prev = buf[tmp.stat.core_number].curr;
            buf[tmp.stat.core_number].curr = tmp.stat;
        }
        sem_post(&stats_mutex);

        sem_wait(&results_mutex);
        for (unsigned int i = 0; i < core_number; i++) {
            double const result = calculate_core_usage_from_proc_stat_t(buf[i].prev, buf[i].curr);
            data_t result_data = {0};
            result_data.result.core_number = i;
            result_data.result.result = result;
            ringbuffer_add(results, result_data);
        }
        sem_post(&results_mutex);
        sleep(1);
    }
    return NULL;
}

void *printer_func() {
    for (;;) {
        sem_wait(&results_mutex);
        if (results->write_index < results->read_index)
            results->read_index = 0;

        while (results->read_index < results->write_index) {
            data_t tmp = {0};
            ringbuffer_get(results, &tmp);
            printf("Core %d: %.2f%%\n", tmp.result.core_number, tmp.result.result);
        }
        printf("\n");
        sem_post(&results_mutex);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    read_cores_num(&core_number, "/proc/stat");
    buf = malloc(sizeof(stat_packet_t) * core_number);
    sem_init(&stats_mutex, 0, 1);
    sem_init(&results_mutex, 0, 1);
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
    sem_destroy(&stats_mutex);
    sem_destroy(&results_mutex);
    free(buf);
}