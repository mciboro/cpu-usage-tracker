/**
 * @file reader.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Header file containing functions which help read 
 * and extract informations from /proc/stat file.
 * @version 0.1
 * @date 2023-02-13
 *
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ring_buffer/ring_buffer.h"
#include "proc_stat.h"

#define MAX_LINE_SIZE 256 // Max size of one line read from file

/**
 * @brief Function that reads proc stat file and puts
 * formatted data into ring buffer.
 *
 * @param rbuf - Pointer to ring buffer
 * @param proc_stat_dir - Directory of proc stat file
 * @return unsigned int
 */
unsigned int read_proc_stat(ring_buffer_t *const rbuf, char const *const proc_stat_dir);