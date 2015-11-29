#pragma once
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>


#define MAX_TIME 5



/*stale zwiazane z pamiecia wspoldzielona*/
#define HEAD 0
#define TAIL 1

/*Stale zwiazane z semaforami*/
#define SEM_NR 3

#define FULL 0
#define EMPTY 1
#define MUTEX 2

#define MTXDOWN -1
#define MTXUP 1


#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};


int init_semaphores(int semid, int m);
int init_shared_memory(int shmid, int m);
int init_random(int seed);

int magic(int semid, int which, int val);
int calc_next_place(int old, int m);

int die(char *str);
int _die_hard(char *str, int who);
#define die_hard(x) (_die_hard(x, whoami))

void print_buffer(int *src, int m);
