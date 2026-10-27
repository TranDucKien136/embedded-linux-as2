/*
 * sensor_thread.c - Read sensor data and push it into the circular buffer
 * Author: Tran Duc Kien - SE183502
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "sensor_thread.h"
#include "../driver/sms_sensor.h"

void *sensor_thread_func(void *arg) {
    struct sensor_args *a = (struct sensor_args *)arg;
    struct sensor_data d;
    char line[128];
    int fd;
    ssize_t n;

    fd = open(a->dev_path, O_RDONLY);

    if (fd < 0) {
        perror("sensor_thread: open");
        return NULL;
    }

    if (ioctl(fd, SMS_SET_INTERVAL, &a->interval_ms) < 0) {
        perror("WARN: sensor_thread ioctl SMS_SET_INTERVAL");
    }

    while (*a->running) {
        n = read(fd, line, sizeof(line) - 1);

        if (n < 0) {
            if (errno == EINTR || errno == EAGAIN)
                continue;

            perror("sensor_thread: read");
            break;
        }

        if (n == 0)
            continue;

        line[n] = '\0';

        if (sscanf(line, "%lld,%f,%f",
                   &d.timestamp_ms,
                   &d.temperature,
                   &d.humidity) != 3) {
            continue;
        }

        if (cb_push(a->cb, &d) < 0) {
            fprintf(stderr, "WARN: buffer day, bo mau\n");
        }

        usleep(a->interval_ms * 1000);
    }

    close(fd);
    return NULL;
}
