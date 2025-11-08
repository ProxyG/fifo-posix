#ifndef HANDLERS_CLI_H
#define HANDLERS_CLI_H

#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t reveil = 0;

void hand_reveil(int sig) {
    reveil = 1;
}

#endif