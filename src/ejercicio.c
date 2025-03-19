#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void) {
    int var1, var2;
    pid_t suma, resta;

    // Solicitar las dos variables
    printf("Introduce variable 1: ");
    scanf("%d", &var1);
    printf("Introduce variable 2: ");
    scanf("%d", &var2);

    if((suma=fork()) > 0){
        resta = fork();
    }
    if(suma == 0){
        printf("Soy el proceso suma y la suma es %d\n", var1+var2);
    }else if(resta == 0){
        printf("Soy el proceso resta y la resta es %d\n", var1-var2);
    }else // Proceso padre
    {
        while(wait(NULL) > 0);
    }

    return 0;
}
