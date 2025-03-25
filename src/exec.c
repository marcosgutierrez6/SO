#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Proceso hijo
        puts("Soy el hijo, voy a ejecutar ls\n");
        execlp("/bin/ls", "ls", "-l", NULL);
        // Si execlp falla, llegaremos aquí
        puts("Error al ejecutar ls\n");
    } else {
        // Proceso padre
        puts("Soy el padre, esperando a que el hijo termine\n");
        wait(NULL);
        puts("El hijo ha terminado\n");
    }
    
    return 0;
}
