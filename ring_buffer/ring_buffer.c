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
        return 0;
    } else {
        printf("Size of ring buffer must be non zero!\n");
        return 1;
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

unsigned int ringbuffer_add(ring_buffer_t *const rbuf, data_t const src, DataType const type) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return 1;
    }

    if (type == STAT) {
        rbuf->data[rbuf->write_index].stat = src.stat;
    } else if (type == RESULT) {
        rbuf->data[rbuf->write_index].result = src.result;
    }

    if ((rbuf->write_index + 1) < RING_BUFFER_SIZE) {
        ++rbuf->write_index;
    } else {
        rbuf->write_index = 0;
    }

    return 0;
}

unsigned int ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data, DataType const type) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return 1;
    }

    if (!data) {
        printf("Data object cannot be void!\n");
        return 2;
    }

    if (type == STAT) {
        data->stat = rbuf->data[rbuf->read_index++].stat;
    } else if (type == RESULT) {
        data->result = rbuf->data[rbuf->read_index++].result;
    }
    return 0;
}