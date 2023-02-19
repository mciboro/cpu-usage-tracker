/**
 * @file ring_buffer_test.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Tests of ringbuffer library.
 * @version 0.1
 * @date 2023-02-13
 *
 */

#include <assert.h>

#include "ring_buffer.h"

int main(void) {
    ring_buffer_t *rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    // assert(!rbuf);
    assert(rbuf->struct_len == 2064);
    assert(rbuf->size == RING_BUFFER_SIZE);
    assert(rbuf->write_index == 0);
    assert(rbuf->read_index == 0);

    ring_buffer_t *rbuf_null = ringbuffer_create(0);
    assert(!rbuf_null);

    ringbuffer_destroy(&rbuf);
    assert(!rbuf);

    assert(ringbuffer_destroy(&rbuf_null) == 1);
    return EXIT_SUCCESS;
}
