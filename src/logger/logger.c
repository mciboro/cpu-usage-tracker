/**
 * @file logger.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Library which is used to retrieve and write to file
 * logs from application
 * @version 0.1
 * @date 2024-07-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "logger.h"

ReturnType_t send_log(const char *format, ...) {
    data_t item = {0};
    item.log.timestamp = time(NULL);
    item.log.text = calloc(1, MAX_LOG_SIZE);
    if (!item.log.text) {
        printf("Memory allocation failed!");
        return MEMORY_ERROR;
    }

    va_list args;
    va_start(args, format);
    vsnprintf(item.log.text, MAX_LOG_SIZE, format, args);
    va_end(args);

    ringbuffer_add(logger_buf, item);
    return SUCCESS;
}