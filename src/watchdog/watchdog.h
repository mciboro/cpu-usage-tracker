/**
 * @file watchdog.h
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Library which is used to monitor activity of 
 * applications threads.
 * @version 0.1
 * @date 2024-07-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "utils.h"
#include "globals.h"
#include "ring_buffer/ring_buffer.h"
#include "logger/logger.h"

#define WATCHDOG_TIMEOUT 2

/**
 * @brief Function used by thread to signalize its activity.
 * 
 * @param th_type 
 * @return ReturnType_t 
 */
ReturnType_t send_heartbeat(ThreadType_t th_type);

/**
 * @brief Function used to update array of threads last activity timestamps.
 * 
 * @param th_type 
 * @param ts 
 * @return ReturnType_t 
 */
ReturnType_t update_heartbeat_status(ThreadType_t th_type, struct timespec ts);

/**
 * @brief Function used to determine if threads are working.
 * 
 * @return ReturnType_t 
 */
ReturnType_t check_heartbeat_status();

/**
 * @brief Function used to initalize array of threads last activity timestamps.
 * 
 * @return ReturnType_t 
 */
ReturnType_t init_heartbeat_status();