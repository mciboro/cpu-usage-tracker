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

    proc_stat_t test_stat = {3, 396374, 5, 64126, 1974716, 334, 0, 9126, 0, 0, 0};
    assert(ringbuffer_add(NULL, test_stat) == 1);
    assert(!ringbuffer_add(rbuf, test_stat));
    assert(rbuf->write_index == 1);
    assert(rbuf->data[rbuf->read_index].guest == test_stat.guest);

    rbuf->write_index = RING_BUFFER_SIZE - 1;
    ringbuffer_add(rbuf, test_stat);
    assert(rbuf->write_index == 0);

    assert(!ringbuffer_destroy(&rbuf));
    assert(!rbuf);

    ring_buffer_t *rbuf_null = ringbuffer_create(0);
    assert(!rbuf_null);

    assert(ringbuffer_destroy(&rbuf_null) == 1);

    return EXIT_SUCCESS;
}
