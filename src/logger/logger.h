/**
 * @file logger.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Library which is used to retrieve and write to file
 * logs from application
 * @version 0.1
 * @date 2024-07-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdarg.h>

#include "utils.h"
#include "globals.h"
#include "ring_buffer/ring_buffer.h"

#define MAX_LOG_SIZE 1024

/**
 * @brief Function used to send logs
 * 
 * @param format 
 * @param ... 
 * @return * ReturnType_t 
 */
ReturnType_t send_log(const char *format, ...);