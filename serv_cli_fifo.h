#ifndef SERV_CLI_FIFO_H
#define SERV_CLI_FIFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define NMAX 100
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

typedef struct {
    pid_t pid_client;
    int nombre;
} Question;

typedef struct {
    int resultat;
} Reponse;

#endif