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

unsigned int ringbuffer_create(ring_buffer_t **_rbuf, unsigned int const size) {
    if (size > 0) {
        ring_buffer_t *rbuf = malloc(sizeof(ring_buffer_t) + sizeof(data_t) * size);
        rbuf->struct_len = sizeof(ring_buffer_t) + sizeof(data_t) * size;
        rbuf->size = size;
        rbuf->read_index = 0;
        rbuf->write_index = 0;
        *_rbuf = rbuf;
        return SUCCESS;
    } else {
        printf("Size of ring buffer must be non zero!\n");
        return INV_ARG;
    }
}

unsigned int ringbuffer_destroy(ring_buffer_t **_rbuf) {
    ring_buffer_t *rbuf = *_rbuf;
    if (rbuf) {
        free(rbuf);
        *_rbuf = NULL;
        return SUCCESS;
    } else {
        printf("Memory already void!\n");
        return VOID_ARG;
    }
}

unsigned int ringbuffer_add(ring_buffer_t *const rbuf, data_t const src) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return VOID_ARG;
    }

    rbuf->data[rbuf->write_index] = src;

    if ((rbuf->write_index + 1) < RING_BUFFER_SIZE) {
        ++rbuf->write_index;
    } else {
        rbuf->write_index = 0;
    }

    return SUCCESS;
}

unsigned int ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return VOID_ARG;
    }

    if (!data) {
        printf("Data object cannot be void!\n");
        return VOID_ARG;
    }

    *data = rbuf->data[rbuf->read_index++];
    return SUCCESS;
}