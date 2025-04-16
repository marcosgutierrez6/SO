#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>

#define LIMITE 1000
static int *variable_compartida;
pthread_mutex_t *lock;

int main(void) {
    
    pthread_mutex_init(&lock, NULL);
    variable_compartida = mmap(NULL, sizeof *variable_compartida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *variable_compartida=0;
    pid_t suma, resta;
    
    suma = fork();

    if(suma>0){
        resta = fork();
    }

    //Problema de la seccion critica

    if(suma==0){
        for(int i = 0; i < LIMITE; i++){
            pthread_mutex_lock(&lock);
            *variable_compartida = *variable_compartida+1;
            pthread_mutex_unlock(&lock);
        }
    }else if(resta==0){
        for(int i = 0; i < LIMITE; i++){
            pthread_mutex_lock(&lock);
            *variable_compartida = *variable_compartida-1;
            pthread_mutex_unlock(&lock);
        }
    }else{
        while(wait(NULL));
        printf("Variable compartida vale %d\n", *variable_compartida);
        munmap(variable_compartida, sizeof *variable_compartida);
        pthread_mutex_destroy(&lock);
        return 0;
    }
    
    return 0;
}