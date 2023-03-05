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

#include "utils.h"

#define RING_BUFFER_SIZE 256

typedef enum DataType { STAT, RESULT } DataType;

/**
 * @brief Union holding one of the types of ring buffer data.
 * 
 */
typedef union data_t {
    proc_stat_t stat;
    core_result_t result;
} data_t;

/**
 * @brief Declaration of ring buffer struct.
 *
 */
typedef struct ring_buffer_t {
    unsigned int size;
    unsigned int write_index;
    unsigned int read_index;
    unsigned int struct_len;
    data_t data[];
} ring_buffer_t;

/**
 * @brief Constructor of ring buffer
 *
 * @param _rbuf Pointer to pointer to ring buffer.
 * @return unsigned int - Status of the operation.
 */
unsigned int ringbuffer_create(ring_buffer_t **_rbuf, unsigned int const size);

/**
 * @brief Destructor of ringbuffer.
 *
 * @param rbuf Pointer to pointer to ring buffer.
 * @return unsigned int - Status of the operation.
 */
unsigned int ringbuffer_destroy(ring_buffer_t **_rbuf);

/**
 * @brief Function that adds element to the buffer.
 *
 * @param rbuf
 * @param src
 * @param type
 * @return unsigned int
 */
unsigned int ringbuffer_add(ring_buffer_t *const rbuf, data_t const src, DataType const type);

/**
 * @brief Function that retrieves element from the buffer.
 * 
 * @param rbuf 
 * @param data 
 * @param type 
 * @return unsigned int 
 */
unsigned int ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data, DataType const type);