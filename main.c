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
#include <stdlib.h>
#include <unistd.h>

#include "analyzer/analyzer.h"
#include "reader/reader.h"
#include "ring_buffer/ring_buffer.h"

typedef struct {
    proc_stat_t prev;
    proc_stat_t curr;
} stat_packet_t;

unsigned int core_number = 0;

ring_buffer_t *stats_buf = NULL, *results_buf = NULL;
sem_t stats_mutex, results_mutex;
stat_packet_t *stat_packets;
double *curr_results;

void *reader_func() {
    for (;;) {
        sem_wait(&stats_mutex);
        read_proc_stat(stats_buf, "/proc/stat");
        sem_post(&stats_mutex);
        sleep(1);
    }
    return NULL;
}

void *analyzer_func() {
    for (;;) {
        sem_wait(&stats_mutex);
        if (stats_buf->write_index < stats_buf->read_index)
            stats_buf->read_index = 0;

        while (stats_buf->read_index < stats_buf->write_index) {
            data_t tmp = {0};
            ringbuffer_get(stats_buf, &tmp);
            stat_packets[tmp.stat.core_number].prev = stat_packets[tmp.stat.core_number].curr;
            stat_packets[tmp.stat.core_number].curr = tmp.stat;
        }
        sem_post(&stats_mutex);

        sem_wait(&results_mutex);
        for (unsigned int i = 0; i < core_number; i++) {
            double const result = calculate_core_usage_from_proc_stat_t(stat_packets[i].prev, stat_packets[i].curr);
            data_t result_data = {0};
            result_data.result.core_number = i;
            result_data.result.result = result;
            ringbuffer_add(results_buf, result_data);
        }
        sem_post(&results_mutex);
        sleep(1);
    }
    return NULL;
}

void *printer_func() {
    for (;;) {
        sem_wait(&results_mutex);
        if (results_buf->write_index < results_buf->read_index)
            results_buf->read_index = 0;

        while (results_buf->read_index < results_buf->write_index) {
            data_t tmp = {0};
            ringbuffer_get(results_buf, &tmp);
            curr_results[tmp.result.core_number] = tmp.result.result;
        }
        
        for(unsigned int i = 0; i < core_number; i++) {
            printf("Core %d: %5.1f%%\t", i, curr_results[i]);
        }
        printf("\r");
        fflush(stdout);

        sem_post(&results_mutex);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    read_cores_num(&core_number, "/proc/stat");
    
    stat_packets = malloc(sizeof(stat_packet_t) * core_number);
    memset(stat_packets, 0, sizeof(stat_packet_t) * core_number);
    curr_results = malloc(sizeof(double) * core_number);
    memset(curr_results, 0, sizeof(double) * core_number);

    sem_init(&stats_mutex, 0, 1);
    sem_init(&results_mutex, 0, 1);
    ringbuffer_create(&stats_buf, RING_BUFFER_SIZE);
    ringbuffer_create(&results_buf, RING_BUFFER_SIZE);

    pthread_t reader, analyzer, printer;
    pthread_create(&reader, NULL, reader_func, NULL);
    pthread_create(&analyzer, NULL, analyzer_func, NULL);
    pthread_create(&printer, NULL, printer_func, NULL);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(reader, NULL);

    ringbuffer_destroy(&stats_buf);
    ringbuffer_destroy(&results_buf);
    sem_destroy(&stats_mutex);
    sem_destroy(&results_mutex);
    free(stat_packets);
    free(curr_results);

    return EXIT_SUCCESS;
}