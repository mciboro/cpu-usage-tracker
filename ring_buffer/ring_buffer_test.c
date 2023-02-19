/**
 * @file ring_buffer_test.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Tests of ringbuffer library.
 * @version 0.1
 * @date 2023-02-13
 *
 */

#include <assert.h>
#include <stdint.h>

#include "ring_buffer.h"

int main(void) {
    ring_buffer_t *rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    // assert(!rbuf);
    assert(rbuf->struct_len == 2064);
    assert(rbuf->size == RING_BUFFER_SIZE);
    assert(rbuf->write_index == 0);
    assert(rbuf->read_index == 0);

    uint8_t test_num = 12;
    assert(ringbuffer_add(NULL, (void *const) & test_num) == 1);
    assert(ringbuffer_add(rbuf, NULL) == 2);
    assert(!ringbuffer_add(rbuf, (void *const) & test_num));
    assert(rbuf->write_index == 1);
    assert(rbuf->data[rbuf->read_index] == (void *const) & test_num);

    rbuf->write_index = RING_BUFFER_SIZE - 1;
    ringbuffer_add(rbuf, (void *const) & test_num);
    assert(rbuf->write_index == 0);

    assert(!ringbuffer_destroy(&rbuf));
    assert(!rbuf);

    ring_buffer_t *rbuf_null = ringbuffer_create(0);
    assert(!rbuf_null);

    assert(ringbuffer_destroy(&rbuf_null) == 1);

    rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    char test_str[] = "text";
    ringbuffer_add(rbuf, (void *const) & test_str);
    assert(!ringbuffer_get(NULL));
    assert(ringbuffer_get(rbuf) == (void *)&test_str);
    assert(rbuf->read_index == 1);

    return EXIT_SUCCESS;
}
