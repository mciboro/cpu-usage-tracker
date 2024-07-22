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

ReturnType_t ringbuffer_create(ring_buffer_t **_rbuf, unsigned int const size) {
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
        printf("Size of ring buffer must be non zero!\n");
        return INV_ARG;
    }
}

ReturnType_t ringbuffer_destroy(ring_buffer_t **_rbuf) {
    ring_buffer_t *rbuf = *_rbuf;
    if (rbuf) {
        sem_destroy(&rbuf->empty);
        sem_destroy(&rbuf->full);
        pthread_mutex_destroy(&rbuf->lock);
        free(rbuf);
        *_rbuf = NULL;
        return SUCCESS;
    } else {
        printf("Memory already void!\n");
        return VOID_ARG;
    }
}

ReturnType_t ringbuffer_add(ring_buffer_t *const rbuf, data_t const src) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
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

ReturnType_t ringbuffer_get(ring_buffer_t *const rbuf, data_t *const data) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return VOID_ARG;
    }

    if (!data) {
        printf("Data object cannot be void!\n");
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

ReturnType_t ringbuffer_timed_get(ring_buffer_t *const rbuf, data_t *const data, unsigned timeout) {
    if (!rbuf) {
        printf("Ring buffer void!\n");
        return VOID_ARG;
    }

    if (!data) {
        printf("Data object cannot be void!\n");
        return VOID_ARG;
    }

    struct timespec ts = {0};
    int sem_status = 0;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        printf("Unable to get time!");
        return RET_ERROR;
    }
    ts.tv_sec += timeout;

    sem_status = sem_timedwait(&rbuf->full, &ts);
    if (sem_status == -1) {
        if (errno == ETIMEDOUT) {
            return WAIT_TIMEOUT;
        } else {
            return RET_ERROR;
        }
    } else {
        pthread_mutex_lock(&rbuf->lock);
        *data = rbuf->data[rbuf->read_index];
        rbuf->read_index = (rbuf->read_index + 1) % rbuf->size;
        pthread_mutex_unlock(&rbuf->lock);
        sem_post(&rbuf->empty);
    }

    return SUCCESS;
}
