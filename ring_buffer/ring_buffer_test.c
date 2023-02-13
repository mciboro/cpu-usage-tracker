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

int main(void)
{
    ring_buffer_t *rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    // assert(rbuf == NULL);
    assert(rbuf->struct_len == 2064);
    return EXIT_SUCCESS;
}
