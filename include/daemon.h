/*
** DeamonTemplate PROJECT, 2021
** daemon.h
** File description:
** Deaemon header file
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#ifndef DAEMON_H_
    #define DAEMON_H_

#ifndef DEAMON_NAME
    #define DEAMON_NAME "my_deamon"
#endif

#ifndef DEAMON_LOG_DEBUG
    #define DEAMON_LOG_DEBUG false
#endif

typedef struct states_s {
    bool stop_requiered;
} states_t;

/* Signal Handler Prototypes */
void signal_handler(int sig);

#endif