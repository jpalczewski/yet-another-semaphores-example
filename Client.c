#include "Client.h"


void clientsay(char* str, pid_t whoami)
{
    printf(ANSI_COLOR_GREEN"[P:%d]\t%s"ANSI_COLOR_RESET"\n", whoami, str);
}

#define l(x) (clientsay(x,whoami))

void Client(int shmid, int semid, int m,int elem_to_process)
{
    pid_t whoami = getpid();
    char str[100];
    int item,*buffer;
    l("Procedura klienta zostala uruchomiona.");
    
    if(init_random(whoami)==-1) die_hard("init_random() zwrocilo blad.");
    buffer = shmat(shmid, 0, 0);
    if(buffer==(int*)-1) die_hard("shmat() zwrocilo blad.");
    
    while(--elem_to_process>=0)
    {
        l("Ciekawe czy mam cos w magazynie...");
        if(magic(semid, FULL, -1)==-1) die_hard("P(FULL) zwrocilo blad.");
        {
            l("Cos jest! Czekam na mozliwosc odczytu.");
            if(magic(semid, MUTEX, -1)==-1) die_hard("P(MUTEX) zwrocilo blad.");
            {
                item = buffer[buffer[HEAD]];
                buffer[buffer[HEAD]] = -1;
                buffer[HEAD] = calc_next_place(buffer[HEAD],m);
                //print_buffer(buffer, m);   
            }
            if(magic(semid, MUTEX, 1)==-1) die_hard("V(MUTEX) zwrocilo blad.");
            if(magic(semid, EMPTY, 1)==-1) die_hard("V(EMPTY) zwrocilo blad.");
        }
        sprintf(str, "Otrzymalem element %d. Pora go sprzedac w 3 sekundy.", item);
        l(str);
        
        sleep(3);
    }
    
    l("Procedura klienta zostala zakonczona.");
    return;
}
