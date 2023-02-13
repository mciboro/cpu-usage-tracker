/**
 * @file ring_buffer.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Implementation of functions related to ringbuffer
 * library.
 * @version 0.1
 * @date 2023-02-13
 *
 */

#include "ring_buffer.h"

ring_buffer_t *ringbuffer_create(unsigned int size)
{
    if (size > 0) {
        ring_buffer_t *rbuf = malloc(sizeof(ring_buffer_t) + sizeof(proc_stat_t *) * size);
        rbuf->struct_len = sizeof(ring_buffer_t) + sizeof(proc_stat_t *) * size;
        return rbuf;
    } else {
        printf("Size of ring buffer must be non zero!\n");
        return NULL;
    }
}