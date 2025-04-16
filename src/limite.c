#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define LIMITE 10000

int main() {
    int *shared_memory = mmap(NULL, sizeof(int) * 4, PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    shared_memory[0] = 0;  // valor
    shared_memory[1] = 0;  // flag[0]
    shared_memory[2] = 0;  // flag[1]
    shared_memory[3] = 0;  // turn

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Proceso 0: suma
        for (int i = 0; i < LIMITE; i++) {
            shared_memory[1] = 1;        // flag[0]
            shared_memory[3] = 1;        // turn = 1
            while (shared_memory[2] && shared_memory[3] == 1);  // espera activa

            // sección crítica
            shared_memory[0] += 1;
            printf("Suma: %d\n", shared_memory[0]);

            shared_memory[1] = 0;        // flag[0] = 0
        }
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Proceso 1: resta
        for (int i = 0; i < LIMITE; i++) {
            shared_memory[2] = 1;        // flag[1]
            shared_memory[3] = 0;        // turn = 0
            while (shared_memory[1] && shared_memory[3] == 0);  // espera activa

            // sección crítica
            shared_memory[0] -= 1;
            printf("Resta: %d\n", shared_memory[0]);

            shared_memory[2] = 0;        // flag[1] = 0
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("\nValor final: %d\n", shared_memory[0]);

    munmap(shared_memory, sizeof(int) * 4);
    return 0;
}
