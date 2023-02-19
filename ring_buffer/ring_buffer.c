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

ring_buffer_t *ringbuffer_create(unsigned int const size) {
    if (size > 0) {
        ring_buffer_t *rbuf = malloc(sizeof(ring_buffer_t) + sizeof(void *) * size);
        rbuf->struct_len = sizeof(ring_buffer_t) + sizeof(void *) * size;
        rbuf->size = size;
        rbuf->read_index = 0;
        rbuf->write_index = 0;
        return rbuf;
    } else {
        printf("Size of ring buffer must be non zero!\n");
        return NULL;
    }
}

unsigned int ringbuffer_destroy(ring_buffer_t **_rbuf) {
    ring_buffer_t *rbuf = *_rbuf;
    if (rbuf) {
        free(rbuf);
        *_rbuf = NULL;
        return 0;
    } else {
        printf("Memory already void!\n");
        return 1;
    }
}