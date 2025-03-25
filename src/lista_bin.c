#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

int main() {
    char comando[256];
    char ruta_completa[512];
    int continuar = 1;
    
    while(continuar) {
        printf("\nIntroduce un comando (o 'salir' para terminar): ");
        scanf("%s", comando);
        
        if(strcmp(comando, "salir") == 0) {
            continuar = 0;
            continue;
        }
        
        // Construir la ruta completa del comando
        strcpy(ruta_completa, "/bin/");
        strcat(ruta_completa, comando);
        
        pid_t pid = fork();
        
        if (pid == 0) {
            // Proceso hijo
            printf("Ejecutando comando: %s\n", ruta_completa);
            execlp(ruta_completa, comando, NULL);
            // Si execlp falla, llegaremos aquí
            printf("Error al ejecutar el comando\n");
        } else {
            // Proceso padre
            printf("Esperando a que termine el comando...\n");
            wait(NULL);
            printf("Comando completado\n");
        }
    }
    
    printf("Programa terminado\n");
    return 0;
}