#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int tuberia1[2], tuberia2[2];
    pid_t pid;
    int num1, num2, suma;

    // Crear las tuberías
    if (pipe(tuberia1) == -1 || pipe(tuberia2) == -1) {
        perror("Error al crear las tuberías");
    }

    // Crear el proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
    }

    if (pid == 0) {  // Proceso hijo
        while(1) {  // Bucle infinito para el hijo
            // Cerrar extremos no utilizados
            close(tuberia1[1]);  // Cierra el extremo de escritura de la primera tubería
            close(tuberia2[0]);  // Cierra el extremo de lectura de la segunda tubería

            // Leer los números de la primera tubería
            read(tuberia1[0], &num1, sizeof(int));
            read(tuberia1[0], &num2, sizeof(int));

            // Calcular la suma
            suma = num1 + num2;

            // Enviar el resultado por la segunda tubería
            write(tuberia2[1], &suma, sizeof(int));

            // Cerrar los extremos restantes
            close(tuberia1[0]);
            close(tuberia2[1]);
        }
    } else {  // Proceso padre
        while(1) {
            // Cerrar extremos no utilizados
            close(tuberia1[0]);  // Cierra el extremo de lectura de la primera tubería
            close(tuberia2[1]);  // Cierra el extremo de escritura de la segunda tubería

            // Leer los números por teclado
            printf("Ingrese el primer número (-1 para salir): ");
            scanf("%d", &num1);
            if (num1 == -1) {
                // Enviar SIGKILL al proceso hijo
                kill(pid, SIGKILL);
                break;
            }
            
            printf("Ingrese el segundo número: ");
            scanf("%d", &num2);

            // Enviar los números por la primera tubería
            write(tuberia1[1], &num1, sizeof(int));
            write(tuberia1[1], &num2, sizeof(int));

            // Leer el resultado de la segunda tubería
            read(tuberia2[0], &suma, sizeof(int));

            // Mostrar el resultado
            printf("La suma es: %d\n", suma);

            // Cerrar los extremos restantes
            close(tuberia1[1]);
            close(tuberia2[0]);
        }

        // Esperar a que el hijo termine
        wait(NULL);
    }

    return 0;
}
