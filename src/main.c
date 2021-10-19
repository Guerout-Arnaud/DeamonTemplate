/*
** DeamonTemplate PROJECT, 2021
** main.c
** File description:
** Main file for Daemon
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/



#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdbool.h>

#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "logger/logger.h"
#include "daemon.h"

logger_t *logger = NULL;

states_t deamon_service_state = {
    .stop_requiered = false
};

void create_logger_instance()
{
    char *log_file = NULL;
    struct tm gmt = {0};
    time_t elapsed = time(NULL);


    gmtime_r(&elapsed, &gmt);
    mkdir(DEAMON_NAME, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    asprintf(&log_file,
            "%s/%d-%02d-%02d_%02d-%02d.log", DEAMON_NAME, gmt.tm_year + 1900,
            gmt.tm_mon + 1, gmt.tm_mday, gmt.tm_hour, gmt.tm_min);
    printf("%s\n", log_file);
    logger = create_logger(true, true, log_file, (bool)DEAMON_LOG_DEBUG);
}

int main(void) {
        
    pid_t pid = 0;
    pid_t sid = -1;

    create_logger_instance();

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Daemon is starting\n"));

    if ((bool)DEAMON_LOG_DEBUG == false)
        pid = fork();
    else {
        log_msg(logger, Log_INFO, asprintf(&logger->msg, "Debug Enable, Fork won't be executed, process PID is %d\n", getpid()));
    }

    if (pid < 0) {
        log_msg(logger, Log_ERROR, asprintf(&logger->msg, "Fork failed. Deamon stopped\n"));
        delete_logger(logger);
        return (-1);
    }
    
    if ((bool)DEAMON_LOG_DEBUG == false && pid > 0) {
        log_msg(logger, Log_INFO, asprintf(&logger->msg, "Fork succeed. Child PID is %d. Closing parent\n", pid));
        delete_logger(logger);
        return (0);
    }

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Starting child process\n"));

    umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    sid = setsid();
    if (sid < 0) {
        log_msg(logger, Log_ERROR, asprintf(&logger->msg, "Error while creating a new session. Deamon stopped"));
        delete_logger(logger);
        return (-1);
    }

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Deamon session created\n"));

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Closing standard outputs.\n"));

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Standard outputs closed.\n"));

    /* Init */
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Daemon is now running\n"));

    /* Run */
    while (1) {
       sleep(1);
    }
    return (0);
}
