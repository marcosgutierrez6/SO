#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define NUM_DATOS 10
#define MAX_VALOR 50

// puntero al array compartido
static int *datos;

// resultados compartidos
static int resultado_suma = 0;
static unsigned long long resultado_producto = 1;

// mutex para proteger las actualizaciones de resultado_suma y resultado_producto
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// prototipos de las funciones de hilo
void *sumar(void *arg);
void *multriplicar(void *arg);

int main()
{
    pid_t pid;
    int fd[2];

    // crear la tubería
    if (pipe(fd) < 0)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // asignar memoria compartida para los datos
    datos = mmap(NULL,
                 NUM_DATOS * sizeof(int),
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS,
                 -1, 0);
    if (datos == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    puts("[Padre] Generando números aleatorios y escribiendo en el array");
    for (int i = 0; i < NUM_DATOS; i++)
    {
        datos[i] = rand() % MAX_VALOR + 1;
        printf("[Padre] Número %2d: %d\n", i + 1, datos[i]);
    }

    // crear el proceso hijo
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        close(fd[0]); // cierra extremo de lectura

        printf("\n[Hijo] Proceso hijo iniciado (PID: %d)\n", getpid());
        // mostrar los números recibidos
        for (int i = 0; i < NUM_DATOS; i++)
        {
            printf("[Hijo] El número %2d es %d\n", i + 1, datos[i]);
        }

        // crear hilos
        pthread_t th_suma, th_producto;
        pthread_create(&th_suma, NULL, sumar, NULL);
        pthread_create(&th_producto, NULL, multriplicar, NULL);

        // esperar a que acaben
        pthread_join(th_suma, NULL);
        pthread_join(th_producto, NULL);

        // enviar resultados al padre
        write(fd[1], &resultado_suma, sizeof(resultado_suma));
        write(fd[1], &resultado_producto, sizeof(resultado_producto));
        close(fd[1]);

        // limpieza
        pthread_mutex_destroy(&mutex);
        munmap(datos, NUM_DATOS * sizeof(int));
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(fd[1]); // cierra extremo de escritura

        // espera a que termine el hijo
        wait(NULL);

        // leer resultados
        int suma_recibida;
        unsigned long long prod_recibido;
        read(fd[0], &suma_recibida, sizeof(suma_recibida));
        read(fd[0], &prod_recibido, sizeof(prod_recibido));
        close(fd[0]);

        // mostrar números originales
        printf("\n[Padre] Números originales recibidos:\n");
        for (int i = 0; i < NUM_DATOS; i++)
        {
            printf("[Padre] Número %2d: %d\n", i + 1, datos[i]);
        }

        // mostrar suma, producto y promedio
        printf("\n[Padre] Suma     = %d\n", suma_recibida);
        printf("[Padre] Producto = %llu\n", prod_recibido);
        double promedio = (double)suma_recibida / NUM_DATOS;
        printf("[Padre] Promedio = %.2f\n", promedio);

        // limpieza
        munmap(datos, NUM_DATOS * sizeof(int));
        return 0;
    }
}

// función que calcula la suma
void *sumar(void *arg)
{
    for (int i = 0; i < NUM_DATOS; i++)
    {
        pthread_mutex_lock(&mutex);
        resultado_suma += datos[i];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// función que calcula el producto
void *multriplicar(void *arg)
{
    for (int i = 0; i < NUM_DATOS; i++)
    {
        pthread_mutex_lock(&mutex);
        resultado_producto *= datos[i];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}