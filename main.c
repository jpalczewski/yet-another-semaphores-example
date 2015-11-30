#include "Producer.h"
#include "Client.h"
#include "SOIUtility.h"

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int spawn(int id, int n, int m, int k, int shmid, int semid)
{
    pid_t pid;
            
    pid = fork();
    if(pid == 0)
        Producer(id, m,k, shmid, semid);
    return pid;
 }


int main(int argc, char **argv)
{
    int n, m, k, shmid, semid, producers;
    key_t ipckey;
    char filepath[PATH_MAX];
    union semun ignored;
    
    printf("Synchronizacja procesow z wykorzystaniem semaforow\n");
    if(argc != 4)
    {
        printf( "Uzycie: %s n m k\n"
                "n - liczba producentow\n"
                "m - rozmiar kolejki\n"
                "k - liczba jednostek towaru, czyli liczb, ktore powinien wstawic kazdy producent do bufora\n", argv[0]);
        return 1;
    }
    
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    k = atoi(argv[3]);
    
    /*
        sprawdamy wszystkie warunki niezbedne do przejscia dalej.
    */
    if(n <= 0) return die("n <= 0");
    if(m <= 0) return die("m <= 0");
    if(k <= 0) return die("k <= 0");
    if((getpagesize()!=4096) || (sizeof(int)!=4)) return die("Ten program nie zadziala na tym komputerze ze wzgledu na egzotyczne rozmiary zmiennych.");
    if(n>1022) return die("Ta wersja obsluguje kolejki do 1022 elementow."); 
    producers = n;
    
    /*
        Do dziela - zdobadzmy klucz       
    */
    //w manie prosza o accessible filepath, takze zdobadzmy je:
    realpath(argv[0], filepath);
    ipckey = ftok(filepath, n);
    if(ipckey == -1) return die("ftok zwrocilo blad");
    
    /*
        alokacja pamieci wspoldzielonej & semaforow.
    */
    shmid = shmget(ipckey, getpagesize(), IPC_CREAT | S_IRUSR | S_IWUSR | IPC_EXCL); //flagi: tworzenie, uprawnienia do czytania&pisania, wylacznosc.
    if(shmid == -1) return die("shmget zwrocilo blad");
    
    semid = semget(ipckey, SEM_NR, IPC_CREAT | S_IRUSR | S_IWUSR | IPC_EXCL);
    if(semid == -1) return die("semget zwrocilo blad");
    
    if(init_semaphores(semid, m)==-1) return die("init_sempahores zwrocilo blad");
    if(init_shared_memory(shmid, m)==-1) return  die("init_shared_memory zwrocilo blad");
    
    puts("Przerwa techniczna. Wcisnij enter, aby kontynuowac.");
    getchar();
    while(--producers>=0)   
        if(spawn(producers, n, m, k, shmid, semid)==0) return 0;
    
    //void Client(int shmid, int semid, int m,int elem_to_process)
    Client(shmid, semid, m, n*k);
    
    shmctl(shmid, IPC_RMID, 0);   
    semctl(semid, SEM_NR, IPC_RMID, ignored);
}
