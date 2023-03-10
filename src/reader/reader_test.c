/**
 * @file reader_test.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Tests of reader library.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#include <assert.h>
#include <stdlib.h>

#include "reader.h"

/**
 * @brief Main function for reader library unit tests.
 *
 * @return int
 */
int main(void) {
    ring_buffer_t *rbuf = NULL;
    assert(!ringbuffer_create(&rbuf, RING_BUFFER_SIZE));
    assert(read_proc_stat(NULL, "../src/reader/proc_stat_example1.txt") == VOID_ARG);
    assert(read_proc_stat(rbuf, NULL) == INV_ARG);
    assert(read_proc_stat(rbuf, "/proc/null") == FILE_ERROR);
    assert(!read_proc_stat(rbuf, "../src/reader/test_srcs/proc_stat_example1.txt"));
    assert(rbuf->write_index == 4);
    assert(rbuf->data[0].stat.core_number == 0);
    assert(rbuf->data[0].stat.user == 155241);
    assert(rbuf->data[0].stat.iowait == 2216);
    assert(rbuf->data[3].stat.core_number == 3);
    assert(rbuf->data[3].stat.softirq == 280);
    assert(rbuf->data[3].stat.steal == 0);
    ringbuffer_destroy(&rbuf);

    assert(!ringbuffer_create(&rbuf, RING_BUFFER_SIZE));
    assert(!read_proc_stat(rbuf, "../src/reader/test_srcs/proc_stat_example2.txt"));
    assert(rbuf->write_index == 8);
    assert(rbuf->data[0].stat.core_number == 0);
    assert(rbuf->data[0].stat.user == 394221);
    assert(rbuf->data[0].stat.iowait == 453);
    assert(rbuf->data[2].stat.core_number == 2);
    assert(rbuf->data[2].stat.idle == 1974716);
    assert(rbuf->data[2].stat.irq == 0);
    assert(rbuf->data[7].stat.core_number == 7);
    assert(rbuf->data[7].stat.softirq == 3780);
    assert(rbuf->data[7].stat.steal == 0);
    ringbuffer_destroy(&rbuf);

    assert(!ringbuffer_create(&rbuf, RING_BUFFER_SIZE));
    assert(!read_proc_stat(rbuf, "../src/reader/test_srcs/proc_stat_corrupted.txt"));
    assert(rbuf->write_index == 0);
    ringbuffer_destroy(&rbuf);

    return SUCCESS;
}