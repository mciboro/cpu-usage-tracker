/**
 * @file watchdog.c
 * @author Michał Ciborowski (mciborowski.it@gmail.com)
 * @brief Implementation of functions related to watchdog 
 * library
 * @version 0.1
 * @date 2024-07-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "watchdog.h"

ReturnType_t send_heartbeat(ThreadType_t th_type) {
    data_t data = {0};
    data.heartbeat.thread_type = th_type;
    if (clock_gettime(CLOCK_REALTIME, &data.heartbeat.timestamp) == -1) {
        send_log("Unable to get time!");
        return RET_ERROR;
    }
    ringbuffer_add(watchdog_buf, data);
    return SUCCESS;
}

ReturnType_t update_heartbeat_status(ThreadType_t th_type, struct timespec ts) {
    hb_status_arr[th_type] = ts;
    return SUCCESS;
}

ReturnType_t check_heartbeat_status() {
    struct timespec now = {0};
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
        send_log("Unable to get time!");
        return RET_ERROR;
    }

    double time_diff = 0.0;
    for (long unsigned i = 0; i < sizeof(hb_status_arr) / sizeof(struct timespec); i++) {
        time_diff = (now.tv_sec - hb_status_arr[i].tv_sec) + (now.tv_nsec - hb_status_arr[i].tv_nsec) / 1e9;
        if (time_diff > WATCHDOG_TIMEOUT) {
            send_log("Thread %s has not responded with heartbeat in %.3f seconds. Ending program now!",
                     ThreadNames[i], time_diff);
            return WATCHDOG_TIMEOUT;
        }
    }
    return SUCCESS;
}

ReturnType_t init_heartbeat_status() {
    struct timespec now = {0};
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
        send_log("Unable to get time!");
        return RET_ERROR;
    }

    for (long unsigned i = 0; i < sizeof(hb_status_arr); i++) {
        hb_status_arr[i] = now;
    }
    return SUCCESS;
}
