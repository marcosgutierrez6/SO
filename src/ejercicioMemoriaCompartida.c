#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>

static int *shared_memory;
static pid_t proceso_lee, proceso_escribe;

void manejador_signal(int signum) {
    if (signum == SIGKILL) {
        printf("Proceso %d recibió SIGKILL\n", getpid());
        exit(0);
    }
}

int main(void) {
    char tecla;
    
    // Configurar el manejador de señales
    signal(SIGKILL, manejador_signal);
    
    // Crear memoria compartida
    shared_memory = (int *)mmap(NULL, sizeof *shared_memory, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_memory = 0;

    // Crear proceso lector
    proceso_lee = fork();
    if (proceso_lee == 0) {
        printf("Proceso lector (PID: %d) - Ingrese un número: ", getpid());
        scanf("%d", shared_memory);
        exit(0);
    }

    // Esperar a que el proceso lector termine
    wait(NULL);

    // Crear proceso escritor
    proceso_escribe = fork();
    if (proceso_escribe == 0) {
        printf("Proceso escritor (PID: %d) - El valor leído es: %d\n", getpid(), *shared_memory);
        exit(0);
    }

    // Proceso padre
    printf("Proceso padre (PID: %d) - Esperando a que el proceso escritor termine...\n", getpid());
    wait(NULL);

    printf("\nPresione una tecla para terminar los procesos...");
    scanf(" %c", &tecla);

    // Enviar SIGKILL a los procesos
    kill(proceso_lee, SIGKILL);
    puts("Proceso lee: Terminando...");
    kill(proceso_escribe, SIGKILL);
    puts("Proceso escribe: Terminando...");
    puts("Proceso padre: Terminando...");
    return 0;
}