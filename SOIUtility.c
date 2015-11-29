#include "SOIUtility.h"



int init_semaphores(int semid, int m)
{
    union semun argument;
    unsigned short values[3];
    values[FULL] = 0;
    values[EMPTY] = m;
    values[MUTEX] = 1;
    argument.array = values;
    
    return semctl(semid, 0, SETALL, argument);
}

int init_random(int seed2)
{
    time_t seed;
    if(time(&seed)==-1) return -1;
    srandom(seed^seed2);
    return 0;
}

int init_shared_memory(int shmid, int m)
{
    int * buffer;
    int j;
    buffer = shmat(shmid, 0, 0);
    if(buffer==(int*)-1) _die_hard("shmat() zwrocilo blad.",0);
    
    buffer[HEAD] = 2;
    buffer[TAIL] = 2;
    for(j = 2; j < m+2; j++) buffer[j] = -1;
    
    if(shmdt(buffer)==-1) _die_hard("shmdt() zwrocilo blad.",0);
    return 0;
}

int magic(int semid, int which, int val)
{
    struct sembuf operations[1];
    operations[0].sem_num = which;
    operations[0].sem_op = val;
    operations[0].sem_flg = 0;
    /*operations[1].sem_num = MUTEX;
    operations[1].sem_op = mutex;
    operations[1].sem_flg = SEM_UNDO;*/
    
    return semop(semid, operations, 1);
}

void print_buffer(int *src, int m)
{
    int j = 2;
    printf("[DEBUG] Zawartosc: [");
    for(; j < m + 2; j++)
        printf("%d", src[j]);
    puts("]");
    printf("[DEBUG] HEAD: %d, TAIL:%d\n", src[0], src[1]);
    
}

int die(char *str)
{
    printf("[FATAL ERROR] %s\n", str);
    printf("[FATAL ERROR] errno: %d\n", errno);
    return 1;
}

int _die_hard(char *str, int whoami)
{
    printf(ANSI_COLOR_CYAN"[BARDZIEJ FATAL ERROR, pid:%d] %s\n",whoami, str);
    printf("[BARDZIEJ FATAL ERROR, pid:%d] errno: %d"ANSI_COLOR_RESET"\n",whoami, errno);
    _exit(1);
}

int calc_next_place(int old, int m)
{
    int new = old + 1;
    if(new == m+2) new = 2;
    return new;
}
