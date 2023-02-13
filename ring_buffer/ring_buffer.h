/**
 * @file ring_buffer.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file for declaration of ringbuffer library.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "proc_stat/proc_stat.h"

#define RING_BUFFER_SIZE 256

typedef struct ring_buffer_t
{
    unsigned int size;
    unsigned int write_index;
    unsigned int read_index;
    unsigned int struct_len;
    proc_stat_t *stats[];
} ring_buffer_t;

ring_buffer_t *ringbuffer_create(unsigned int size);