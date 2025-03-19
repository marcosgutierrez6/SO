#include <stdio.h>
#include <string.h>

int main(void) {
    char string[100];  // Definimos un tamaño para la cadena
    printf("Introduce una cadena: ");
    scanf("%s", string);  // Leemos la cadena completa

    int length = strlen(string);
    int encontrado[256] = {0};  // Array para marcar los caracteres encontrados

    for (int i = 0; i < length; i++) {
        // Si el caracter ya ha sido marcado, lo omitimos
        if (encontrado[string[i]] == 1) {
            continue;
        }

        int repetido = 0;
        for (int j = i + 1; j < length; j++) {
            if (string[i] == string[j]) {
                repetido = 1;  // Marcamos que hay repetición
                break;
            }
        }

        if (repetido) {
            printf("El caracter '%c' se repite en la palabra %s.\n", string[i], string);
        }

        // Marcamos el caracter como encontrado
        encontrado[string[i]] = 1;
    }

    return 0;
}
