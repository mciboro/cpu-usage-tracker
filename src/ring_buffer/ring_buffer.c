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
        memset(rbuf, 0, sizeof(ring_buffer_t) + sizeof(data_t) * size);
        rbuf->struct_len = sizeof(ring_buffer_t) + sizeof(data_t) * size;
        rbuf->size = size;
        rbuf->read_index = 0;
        rbuf->write_index = 0;
        sem_init(&rbuf->empty, 0, rbuf->size);
        sem_init(&rbuf->full, 0, 0);
        pthread_mutex_init(&rbuf->lock, NULL);
        *_rbuf = rbuf;
        return SUCCESS;
    } else {
        put_log("Size of ring buffer must be non zero!");
        return INV_ARG;
    }
}

unsigned int ringbuffer_destroy(ring_buffer_t **_rbuf) {
    ring_buffer_t *rbuf = *_rbuf;
    if (rbuf) {
        sem_destroy(&rbuf->empty);
        sem_destroy(&rbuf->full);
        pthread_mutex_destroy(&rbuf->lock);
        free(rbuf);
        *_rbuf = NULL;
        return SUCCESS;
    } else {
        put_log("Memory already void!");
        return VOID_ARG;
    }
}

unsigned int ringbuffer_add(ring_buffer_t *const rbuf, data_t const src) {
    if (!rbuf) {
        put_log("Ring buffer void!");
        return VOID_ARG;
    }

    sem_wait(&rbuf->empty);
    pthread_mutex_lock(&rbuf->lock);
    rbuf->data[rbuf->write_index] = src;
    rbuf->write_index = (rbuf->write_index + 1) % rbuf->size;
    pthread_mutex_unlock(&rbuf->lock);
    sem_post(&rbuf->full);

    return SUCCESS;
}

unsigned int ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data) {
    if (!rbuf) {
        put_log("Ring buffer void!");
        return VOID_ARG;
    }

    if (!data) {
        put_log("Data object cannot be void!");
        return VOID_ARG;
    }

    sem_wait(&rbuf->full);
    pthread_mutex_lock(&rbuf->lock);
    *data = rbuf->data[rbuf->read_index];
    rbuf->read_index = (rbuf->read_index + 1) % rbuf->size;
    pthread_mutex_unlock(&rbuf->lock);
    sem_post(&rbuf->empty);

    return SUCCESS;
}