/**
 * @file logger.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "logger.h"
#include <string.h>

ring_buffer_t *logger_buf;

unsigned int put_log(char const *const log) {
    if (!log) {
        printf("Log cannot be null!\n");
        return INV_ARG;
    }
    if (!logger_buf) {
        printf("LOG: %s\n", log);
        return SUCCESS;
    }

    data_t log_data = {0};
    strcpy(log_data.log, log);
    unsigned int status = ringbuffer_add(logger_buf, log_data);
    return status;
}

unsigned int register_logger_buf(unsigned int const size) {
    if (!size) {
        printf("Size cannot be null!\n");
        return INV_ARG;
    }
    return ringbuffer_create(&logger_buf, size);
}

unsigned int destroy_logger_buf() { return ringbuffer_destroy(&logger_buf); }