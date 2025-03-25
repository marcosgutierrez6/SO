#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>

int main() {
    int fd[2];
    pid_t pid;
    char mensaje[] = "Hola desde el proceso hijo";
    char buffer[100];

    // Crear la tubería
    if (pipe(fd) == -1) {
        perror("Error al crear la tubería");
        return 1;
    }

    // Crear el proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    if (pid == 0) {
        // Proceso hijo
        close(fd[0]);  // Cerrar el extremo de lectura
        
        // Escribir en la tubería
        write(fd[1], mensaje, sizeof(mensaje));
        printf("Proceso hijo: He escrito en la tubería\n");
        
        close(fd[1]);  // Cerrar el extremo de escritura
    } else {
        // Proceso padre
        close(fd[1]);  // Cerrar el extremo de escritura
        
        // Esperar a que el hijo termine de escribir
        wait(NULL);
        
        // Leer de la tubería
        read(fd[0], buffer, sizeof(buffer));
        printf("Proceso padre: He leído en la tubería: %s\n", buffer);
        
        close(fd[0]);  // Cerrar el extremo de lectura
    }

    return 0;
} 