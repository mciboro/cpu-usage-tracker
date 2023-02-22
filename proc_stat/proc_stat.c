/**
 * @file proc_stat.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Implementations of functions related to data
 * in /proc/stat file.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#include "proc_stat.h"

unsigned int read_proc_stat(ring_buffer_t *const rbuf, char const *const proc_stat_dir) {
    if (!rbuf) {
        printf("There is no ring buffer!\n");
        return 1;
    }

    if (!proc_stat_dir) {
        printf("There is no proc stat file direction!\n");
        return 2;
    }

    FILE *fptr = fopen(proc_stat_dir, "r");
    if (!fptr) {
        printf("Error while opening proc stat file!\n");
        return 3;
    }

    char tmp_str[MAX_LINE_SIZE] = {0};
    proc_stat_t tmp_line = {0};
    fgets(tmp_str, MAX_LINE_SIZE, fptr); // skip first line of /proc/stat file

    while (fscanf(fptr, "cpu%u %u %u %u %u %u %u %u %u %u %u\n", &tmp_line.core_number, &tmp_line.user,
                  &tmp_line.nice, &tmp_line.system, &tmp_line.idle, &tmp_line.iowait, &tmp_line.irq,
                  &tmp_line.softirq, &tmp_line.steal, &tmp_line.guest, &tmp_line.guest_nice) > 0) {
        proc_stat_t *saved_line = malloc(sizeof(proc_stat_t));
        memcpy(saved_line, &tmp_line, sizeof(proc_stat_t));
        ringbuffer_add(rbuf, (void *const)saved_line);
    }

    fclose(fptr);
    return 0;
}