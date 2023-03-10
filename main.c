/**
 * @file main.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief CPU Usage Tracker
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "analyzer/analyzer.h"
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
    sem_t *stats_mutex;
    ring_buffer_t *stats_buf;
} reader_args_t;

/**
 * @brief Arguments of analyzer function.
 *
 */
typedef struct {
    sem_t *stats_mutex;
    sem_t *results_mutex;
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
    sem_t *results_mutex;
    ring_buffer_t *results_buf;
    double *curr_results;
    unsigned int *core_number;
} printer_args_t;

volatile sig_atomic_t working = true;

/**
 * @brief Reader function called by reader thread.
 *
 * @param reader_args
 * @return void*
 */
void *reader_func(void *reader_args) {
    if (!reader_args) {
        printf("No args in reader func!\n");
        return NULL;
    }

    reader_args_t *args = (reader_args_t *)reader_args;
    while (working) {
        sem_wait(args->stats_mutex);
        read_proc_stat(args->stats_buf, "/proc/stat");
        sem_post(args->stats_mutex);
        sleep(1);
    }
    return NULL;
}

/**
 * @brief Analyzer function called by analyzer thread.
 *
 * @param analyzer_args
 * @return void*
 */
void *analyzer_func(void *analyzer_args) {
    if (!analyzer_args) {
        printf("No args in analyzer func!\n");
        return NULL;
    }

    analyzer_args_t *args = (analyzer_args_t *)analyzer_args;
    while (working) {
        sem_wait(args->stats_mutex);
        if (args->stats_buf->write_index < args->stats_buf->read_index)
            args->stats_buf->read_index = 0;

        while (args->stats_buf->read_index < args->stats_buf->write_index) {
            data_t tmp = {0};
            ringbuffer_get(args->stats_buf, &tmp);
            args->stat_packets[tmp.stat.core_number].prev = args->stat_packets[tmp.stat.core_number].curr;
            args->stat_packets[tmp.stat.core_number].curr = tmp.stat;
        }
        sem_post(args->stats_mutex);

        sem_wait(args->results_mutex);
        for (unsigned int i = 0; i < *args->core_number; i++) {
            double const result =
                calculate_core_usage_from_proc_stat_t(args->stat_packets[i].prev, args->stat_packets[i].curr);
            data_t result_data = {0};
            result_data.result.core_number = i;
            result_data.result.result = result;
            ringbuffer_add(args->results_buf, result_data);
        }
        sem_post(args->results_mutex);
        sleep(1);
    }
    return NULL;
}

/**
 * @brief Printer function called by printer thread.
 *
 * @param printer_args
 * @return void*
 */
void *printer_func(void *printer_args) {
    if (!printer_args) {
        printf("No args in printer func!\n");
        return NULL;
    }

    printer_args_t *args = (printer_args_t *)printer_args;
    while (working) {
        sem_wait(args->results_mutex);
        if (args->results_buf->write_index < args->results_buf->read_index)
            args->results_buf->read_index = 0;

        while (args->results_buf->read_index < args->results_buf->write_index) {
            data_t tmp = {0};
            ringbuffer_get(args->results_buf, &tmp);
            args->curr_results[tmp.result.core_number] = tmp.result.result;
        }

        for (unsigned int i = 0; i < *args->core_number; i++) {
            printf("Core %d: %5.1f%%\t", i, args->curr_results[i]);
        }
        printf("\r");
        fflush(stdout);

        sem_post(args->results_mutex);
        sleep(1);
    }
    return NULL;
}

/**
 * @brief Function needed to end program.
 *
 */
void end_program() { working = false; }

/**
 * @brief Main function of CUT.
 *
 * @return int
 */
int main(void) {
    ring_buffer_t *stats_buf = NULL, *results_buf = NULL;
    sem_t stats_mutex = {0}, results_mutex = {0};
    stat_packet_t *stat_packets = NULL;
    double *curr_results = NULL;
    unsigned int core_number = 0;

    struct sigaction sigint = {0}, sigterm = {0};
    sigint.sa_handler = end_program;
    sigterm.sa_handler = end_program;
    sigaction(SIGINT, &sigint, NULL);
    sigaction(SIGTERM, &sigterm, NULL);

    read_cores_num(&core_number, "/proc/stat");

    stat_packets = malloc(sizeof(stat_packet_t) * core_number);
    memset(stat_packets, 0, sizeof(stat_packet_t) * core_number);
    curr_results = malloc(sizeof(double) * core_number);
    memset(curr_results, 0, sizeof(double) * core_number);

    sem_init(&stats_mutex, 0, 1);
    sem_init(&results_mutex, 0, 1);
    ringbuffer_create(&stats_buf, RING_BUFFER_SIZE);
    ringbuffer_create(&results_buf, RING_BUFFER_SIZE);

    pthread_t reader = {0}, analyzer = {0}, printer = {0};
    reader_args_t reader_args = {&stats_mutex, stats_buf};
    pthread_create(&reader, NULL, reader_func, &reader_args);
    analyzer_args_t analyzer_args = {&stats_mutex, &results_mutex, stats_buf,
                                     results_buf,  stat_packets,   &core_number};
    pthread_create(&analyzer, NULL, analyzer_func, &analyzer_args);
    printer_args_t printer_args = {&results_mutex, results_buf, curr_results, &core_number};
    pthread_create(&printer, NULL, printer_func, &printer_args);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);

    ringbuffer_destroy(&stats_buf);
    ringbuffer_destroy(&results_buf);
    sem_destroy(&stats_mutex);
    sem_destroy(&results_mutex);
    free(stat_packets);
    free(curr_results);
    printf("\nEnd of program\n");

    return SUCCESS;
}