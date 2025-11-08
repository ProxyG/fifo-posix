#ifndef HANDLERS_SERV_H
#define HANDLERS_SERV_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t reveil = 0;

void hand_reveil(int sig) {
    reveil = 1;
}

void ignorer_signal(int sig) {
    /* Ne fait rien - ignore le signal */
}

#endif