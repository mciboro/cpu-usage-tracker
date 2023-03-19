/**
 * @file logger.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file of looger library.
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <semaphore.h>
#include "ring_buffer/ring_buffer.h"

extern ring_buffer_t *logger_buf;

/**
 * @brief Function used for logging purposes.
 * 
 * @param log 
 * @return unsigned int 
 */
unsigned int put_log(char const *const log);

/**
 * @brief Function which initializes ring buffer used for
 * logging purposes.
 *
 * @param size
 * @return unsigned int - Status of the operation
 */
unsigned int register_logger_buf(unsigned int const size);

/**
 * @brief Function which destroys ring buffer used for
 * logging purposes.
 *
 * @return unsigned int
 */
unsigned int destroy_logger_buf();