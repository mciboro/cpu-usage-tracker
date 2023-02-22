/**
 * @file proc_stat_test.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Tests of procstat library.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#include <assert.h>
#include <stdlib.h>

#include "proc_stat.h"
#include "ring_buffer/ring_buffer.h"

/**
 * @brief Main function for procstat library unit tests.
 *
 * @return int
 */
int main(void) {
    ring_buffer_t *rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    assert(read_proc_stat(NULL, "../proc_stat/proc_stat_example1.txt") == 1);
    assert(read_proc_stat(rbuf, NULL) == 2);
    assert(read_proc_stat(rbuf, "/proc/null") == 3);
    assert(!read_proc_stat(rbuf, "../proc_stat/test_srcs/proc_stat_example1.txt"));
    assert(rbuf->write_index == 4);
    assert(((proc_stat_t *const)rbuf->data[0])->core_number == 0);
    assert(((proc_stat_t *const)rbuf->data[0])->user == 155241);
    assert(((proc_stat_t *const)rbuf->data[0])->iowait == 2216);
    assert(((proc_stat_t *const)rbuf->data[3])->core_number == 3);
    assert(((proc_stat_t *const)rbuf->data[3])->softirq == 280);
    assert(((proc_stat_t *const)rbuf->data[3])->guest_nice == 0);
    ringbuffer_destroy(&rbuf);

    rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    assert(!read_proc_stat(rbuf, "../proc_stat/test_srcs/proc_stat_example2.txt"));
    assert(rbuf->write_index == 8);
    assert(((proc_stat_t *const)rbuf->data[0])->core_number == 0);
    assert(((proc_stat_t *const)rbuf->data[0])->user == 394221);
    assert(((proc_stat_t *const)rbuf->data[0])->iowait == 453);
    assert(((proc_stat_t *const)rbuf->data[2])->core_number == 2);
    assert(((proc_stat_t *const)rbuf->data[2])->idle == 1974716);
    assert(((proc_stat_t *const)rbuf->data[2])->irq == 0);
    assert(((proc_stat_t *const)rbuf->data[7])->core_number == 7);
    assert(((proc_stat_t *const)rbuf->data[7])->softirq == 3780);
    assert(((proc_stat_t *const)rbuf->data[7])->guest_nice == 0);
    ringbuffer_destroy(&rbuf);

    rbuf = ringbuffer_create(RING_BUFFER_SIZE);
    assert(!read_proc_stat(rbuf, "../proc_stat/test_srcs/proc_stat_corrupted.txt"));
    assert(rbuf->write_index == 0);
    ringbuffer_destroy(&rbuf);

    return EXIT_SUCCESS;
}