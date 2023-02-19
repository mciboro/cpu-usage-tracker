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

#define RING_BUFFER_SIZE 256

/**
 * @brief Declaration of ring buffer struct.
 *
 */
typedef struct ring_buffer_t
{
    unsigned int size;
    unsigned int write_index;
    unsigned int read_index;
    unsigned int struct_len;
    void *data[];
} ring_buffer_t;

/**
 * @brief Constructor of ring buffer
 *
 * @param size Number of ring buffer elements.
 * @return ring_buffer_t*
 */
ring_buffer_t *ringbuffer_create(unsigned int const size);

/**
 * @brief Destructor of ringbuffer.
 *
 * @param rbuf Pointer to pointer to ring buffer.
 * @return int
 */
unsigned int ringbuffer_destroy(ring_buffer_t **_rbuf);

