/**
 * @file cut.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief CPU Usage Tracker
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cut.h"

void end_program();

volatile sig_atomic_t working = true;

void *reader_func(void *_) {
    UNUSED(_);
   
    send_log("Begin of reader thread!");
    while (working) {
        send_heartbeat(READER);

        read_proc_stat(stats_buf, "/proc/stat");
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
        send_log("No args in analyzer func!");
        return NULL;
    }

    analyzer_args_t *args = (analyzer_args_t *)analyzer_args;

    send_log("Begin of analyzer thread!");
    while (working) {
        send_heartbeat(ANALYZER);
        data_t tmp = {0};

        ringbuffer_get(stats_buf, &tmp);
        args->stat_packets[tmp.stat.core_number].prev = args->stat_packets[tmp.stat.core_number].curr;
        args->stat_packets[tmp.stat.core_number].curr = tmp.stat;

        for (unsigned int i = 0; i < *args->core_number; i++) {
            double const result =
                calculate_core_usage_from_proc_stat_t(args->stat_packets[i].prev, args->stat_packets[i].curr);
            data_t result_data = {0};
            result_data.result.core_number = i;
            result_data.result.result = result;
            ringbuffer_add(results_buf, result_data);
        }
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
        send_log("No args in printer func!");
        return NULL;
    }

    printer_args_t *args = (printer_args_t *)printer_args;

    send_log("Begin of printer thread!");
    while (working) {
        send_heartbeat(PRINTER);
        data_t tmp = {0};

        ringbuffer_get(results_buf, &tmp);
        args->curr_results[tmp.result.core_number] = tmp.result.result;

        for (unsigned int i = 0; i < *args->core_number; i++) {
            printf("Core %d: %5.1f%%\t", i, args->curr_results[i]);
        }
        printf("\r");
        fflush(stdout);
    }
    return NULL;
}

/**
 * @brief Logger function called by logger thread.
 *
 * @param logger_args
 * @return void*
 */
void *logger_func(void *logger_args) {
    if (!logger_args) {
        printf("No args in logger func!\n");
        return NULL;
    }

    logger_args_t *args = (logger_args_t *)logger_args;
    
    FILE *fptr = fopen(args->logger_filename, "w");
    if (!fptr) {
        printf("Error while opening logs file!\n");
        return NULL;
    }

    send_log("Begin of logger thread!");
    while (working) {
        data_t item = {0};
        ringbuffer_get(logger_buf, &item);

        if (item.log.text) {
            struct tm ts;
            char date[80];
            ts = *localtime(&item.log.timestamp);
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &ts);

            fprintf(fptr, "%s: %s\n", date, item.log.text);
            free(item.log.text);
        }
    }

    fclose(fptr);
    return NULL;
}

/**
 * @brief Function called by watchdog thread. 
 * 
 * @param _ 
 * @return void* 
 */
void *watchdog_func(void *_) {
    UNUSED(_);

    send_log("Begin of watchdog thread!");
    init_heartbeat_status();
    while (working) {
        data_t item = {0};
        ReturnType_t ret = ringbuffer_timed_get(watchdog_buf, &item, 1);
        if (ret == SUCCESS) {
            update_heartbeat_status(item.heartbeat.thread_type, item.heartbeat.timestamp);
        } else if (ret != WAIT_TIMEOUT) {
            end_program();
        }

        if (check_heartbeat_status() == WATCHDOG_TIMEOUT) {
            end_program();
        }
    }

    return NULL;
}



/**
 * @brief Function needed to end program.
 *
 */
void end_program() {
    send_log("Ending threads!");

    working = false;
    sem_post(&results_buf->full);
    sem_post(&stats_buf->full);
    sem_post(&logger_buf->full);
}

/**
 * @brief Main function of CUT.
 *
 * @return int
 */
int main(void) {
    stat_packet_t *stat_packets = NULL;
    double *curr_results = NULL;
    unsigned int core_number = 0;

    struct sigaction sigint = {0}, sigterm = {0};
    sigint.sa_handler = end_program;
    sigterm.sa_handler = end_program;
    sigemptyset(&sigint.sa_mask);
    sigemptyset(&sigterm.sa_mask);
    sigint.sa_flags = 0;
    sigterm.sa_flags = 0;
    sigaction(SIGINT, &sigint, NULL);
    sigaction(SIGTERM, &sigterm, NULL);

    read_cores_num(&core_number, "/proc/stat");

    stat_packets = calloc(1, sizeof(stat_packet_t) * core_number);
    curr_results = calloc(1, sizeof(double) * core_number);

    ringbuffer_create(&stats_buf, RING_BUFFER_SIZE);
    ringbuffer_create(&results_buf, RING_BUFFER_SIZE);
    ringbuffer_create(&logger_buf, RING_BUFFER_SIZE);
    ringbuffer_create(&watchdog_buf, RING_BUFFER_SIZE);

    pthread_t reader = {0}, analyzer = {0}, printer = {0}, logger = {0}, watchdog = {0};
    pthread_create(&reader, NULL, reader_func, NULL);
    analyzer_args_t analyzer_args = {stat_packets, &core_number};
    pthread_create(&analyzer, NULL, analyzer_func, &analyzer_args);
    printer_args_t printer_args = {curr_results, &core_number};
    pthread_create(&printer, NULL, printer_func, &printer_args);
    logger_args_t logger_args = {"logs.txt"};
    pthread_create(&logger, NULL, logger_func, &logger_args);
    pthread_create(&watchdog, NULL, watchdog_func, NULL);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);
    pthread_join(logger, NULL);
    pthread_join(watchdog, NULL);

    ringbuffer_destroy(&stats_buf);
    ringbuffer_destroy(&results_buf);
    ringbuffer_destroy(&logger_buf);
    ringbuffer_destroy(&watchdog_buf);
    free(stat_packets);
    free(curr_results);
    printf("\nEnd of program\n");

    return SUCCESS;
}