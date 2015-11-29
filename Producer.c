#include "Producer.h"


void producersay(char* str, pid_t whoami, int id)
{
    printf(ANSI_COLOR_RED"[P:%d,%d]\t%s"ANSI_COLOR_RESET"\n", whoami,id, str);
}

#define l(x) (producersay(x,id,whoami))

void Producer(int id, int m, int k, int shmid, int semid) 
{
    pid_t whoami = getpid();
    int* buffer, timeToWait;
    char str[100];
    l("Producent zostal uruchomiony");
    
    if(init_random(whoami)==-1) die_hard("init_random() zwrocilo blad.");
    buffer = shmat(shmid, 0, 0);
    if(buffer==(int*)-1) die_hard("shmat() zwrocilo blad.");
    
    while(--k>=0)
    {
        timeToWait = random() % MAX_TIME;
        sprintf(str, "Zaczynam produkcje, potrwa %d sekund...", timeToWait);
        l(str);
        sleep(timeToWait);
        l("Skonczylem produkcje, czas zapukac do magazynu.");
        if(magic(semid, EMPTY, -1)==-1) die_hard("P(MUTEX) zwrocilo blad.");
        {
            l("Jestem w magazynie! Czekam na mozliwosc zapisu...");
            if(magic(semid, MUTEX, -1)==-1) die_hard("P(MUTEX) zwrocilo blad.");
            {
                buffer[buffer[TAIL]] = id;
                buffer[TAIL] = calc_next_place(buffer[TAIL], m);   
            }
            if(magic(semid, MUTEX, 1)==-1)  die_hard("V(MUTEX) zwrocilo blad.");
            if(magic(semid, FULL, 1)==-1) die_hard("V(FULL) zwrocilo blad.");   
        }
        l("Produkt zostawiony, wychodze z magazynu.");     
    }
    
    if(shmdt(buffer)==-1) die_hard("shmdt() zwrocilo blad.");
    l("Producent zakonczyl prace");
    return;
}

	
