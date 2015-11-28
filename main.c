#include "producer.h"
#include "client.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int spawn(int n, int m, int k)
{
    int i;
    pid_t pid;
            
    pid = fork();
    if(pid == 0)
        Producer(m,k);
    return pid;
 }


int main(int argc, char **argv)
{
    int n, m, k;
    
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
    
    while(--n>=0)   
        if(spawn(n,m,k)==0) return 0;
    
    Client();
}
