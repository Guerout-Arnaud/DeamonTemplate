/*
** DeamonTemplate PROJECT, 2021
** handler.c
** File description:
** Signal handler to stop Daemon gracefuly
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "logger/logger.h"
#include "daemon.h"

extern logger_t *logger;

extern states_t deamon_service_state;


void signal_handler(int sig)
{
    if (deamon_service_state.stop_requiered == true) {
        log_msg(logger, Log_NONE, asprintf(&logger->msg, BOLD(RED("Force stop requested by user\n")) RESET));
        exit(1);
    }
    
    deamon_service_state.stop_requiered = true;

    log_msg(logger, Log_INFO, asprintf(&logger->msg, "Stopping daemon. Send SigTerm or SigInt to force stop\n"));

    delete_logger(logger);
    if (sig == SIGTERM)
        exit(0);
    else {
        exit(-1);
    }
}