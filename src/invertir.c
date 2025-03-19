#include <stdio.h>
#include <string.h>

int main(void) {
    char str[255], *ptr1, *ptr2, temp;
    printf("Introduce una cadena: ");
    scanf("%s", str);

    // Inicializamos ptr1 al principio de la cadena y ptr2 al final
    ptr1 = str; 
    ptr2 = str;
    
    // Avanzamos ptr2 hasta el último carácter de la cadena
    while (*ptr2 != '\0') {
        ptr2++;
    }
    ptr2--;  // Retrocedemos un paso para que apunte al último carácter válido

    // Intercambiamos los caracteres utilizando los punteros
    while (ptr1 < ptr2) {
        temp = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = temp;

        ptr1++;
        ptr2--;
    }

    printf("Cadena invertida: %s\n", str);
    return 0;
}
