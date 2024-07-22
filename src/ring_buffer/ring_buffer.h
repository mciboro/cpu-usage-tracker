/**
 * @file ring_buffer.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file for declaration of ringbuffer library.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "utils.h"

#define RING_BUFFER_SIZE 256

/**
 * @brief Union holding one of the types of ring buffer data.
 *
 */
typedef union {
    proc_stat_t stat;
    core_result_t result;
    log_t log;
    heartbeat_t heartbeat;
} data_t;

/**
 * @brief Declaration of ring buffer struct.
 *
 */
typedef struct {
    unsigned int size;
    unsigned int write_index;
    unsigned int read_index;
    unsigned int struct_len;
    pthread_mutex_t lock;
    sem_t empty;
    sem_t full;
    data_t data[];
} ring_buffer_t;

/**
 * @brief Constructor of ring buffer
 *
 * @param _rbuf Pointer to pointer to ring buffer.
 * @return ReturnType_t - Status of the operation.
 */
ReturnType_t ringbuffer_create(ring_buffer_t **_rbuf, unsigned int const size);

/**
 * @brief Destructor of ringbuffer.
 *
 * @param rbuf Pointer to pointer to ring buffer.
 * @return ReturnType_t - Status of the operation.
 */
ReturnType_t ringbuffer_destroy(ring_buffer_t **_rbuf);

/**
 * @brief Function that adds element to the buffer.
 *
 * @param rbuf
 * @param src
 * @return ReturnType_t - Status of the operation.
 */
ReturnType_t ringbuffer_add(ring_buffer_t *const rbuf, data_t const src);

/**
 * @brief Function that retrieves element from the buffer.
 *
 * @param rbuf
 * @param data
 * @return ReturnType_t - Status of the operation.
 */
ReturnType_t ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data);

/**
 * @brief Function that retrieves element from the buffer or returns after number
 * of seconds given by parameter timeout if there is nothing to retrieve.
 *
 * @param rbuf
 * @param data
 * @param timeout
 * @return ReturnType_t - Status of the operation.
 */
ReturnType_t ringbuffer_timed_get(ring_buffer_t *const rbuf, data_t *const data, unsigned timeout);
