#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main (void){
    int var1 = 10, var2 = 20;
    pid_t pid;
    printf("Proceso padre con pid: %d\n", getpid());

    pid = fork();
    if(pid > 0){
        puts("Padre no hace nada");
        wait(NULL); // Espera a que el hijo termine
        printf("la var 1 es %d y la var 2 es %d El hijo finaliza", var1, var2);
    }
    else if(pid == 0){
        printf("soy el proceso %d y mi padre es %d\n", getpid(), getppid());
        
        scanf("%d", &var1);
        scanf("%d", &var2);
        printf("La suma es %d\n", var1+var2);
        printf("La resta es %d\n", var1-var2);
    }else{
        perror("Me ha programado el becario, error falta\n");
    }


    return 0;
}