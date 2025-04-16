#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

// Número compartido entre procesos
static int *shared_num;

// Semáforos
static sem_t *sem_leido;     // Indica que se ha leído un número
static sem_t *sem_terminar;  // Indica que ya se puede terminar

int main(void)
{
    pid_t lee, escribe;

    // Reservar memoria compartida para el número y los semáforos
    shared_num = mmap(NULL, sizeof *shared_num, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_leido = mmap(NULL, sizeof *sem_leido, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_terminar = mmap(NULL, sizeof *sem_terminar, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Inicializar semáforos
    sem_init(sem_leido, 1, 0);     // Inicia bloqueado
    sem_init(sem_terminar, 1, 0);  // Inicia bloqueado

    lee = fork();
    if (lee > 0) {
        escribe = fork();
    }

    if (lee == 0) {  // Proceso lee
        printf("Introduce un número:\n");
        scanf("%d", shared_num);

        sem_post(sem_leido);  // Señal al proceso escribe

        sem_wait(sem_terminar);  // Espera a que el proceso escriba termine
        puts("Soy el proceso lee: me largo!");
        exit(0);
    } else if (escribe == 0) {  // Proceso escribe
        sem_wait(sem_leido);  // Espera a que el número esté listo

        printf("El usuario ha escrito: %d\n", *shared_num);
        puts("Pulsa cualquier tecla para terminar");
        getchar();  // Para consumir el \n pendiente del scanf anterior
        getchar();  // Espera al usuario
        puts("Soy el proceso escribe: me largo!!");

        sem_post(sem_terminar);  // Señal al proceso lee
        exit(0);
    } else {
        // Proceso padre espera que los hijos terminen
        while (wait(NULL) > 0);
    }

    // Liberar recursos
    sem_destroy(sem_leido);
    sem_destroy(sem_terminar);
    puts("Soy el proceso padre: me largo!!!");
    return 0;
}