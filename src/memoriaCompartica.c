#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

static int *shared_memory;

int main(void) {
    pid_t hijo;
    
    shared_memory = (int *)mmap(NULL, sizeof *shared_memory, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_memory = 0;

    hijo = fork();

    if (hijo == 0) {
        puts("Soy el hijo, dame el valor de la variable:");
        scanf("%d", *&shared_memory);
        exit(0);
    }else if(hijo > 0){
        puts("Soy el padre, quedo a la espera");
        wait(NULL);
        printf("Soy el proceso padre y el hijo ha cambiado el valor de la variable a %d\n", *shared_memory);
    }
    return 0;
}