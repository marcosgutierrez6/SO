#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>


#define LIMITE 1000000
void *incrementa();
void *decrementa();

// Variable compartida entre los hilos
static int global_var=0;

// Mutex para sincronizar el acceso a la variable compartida
pthread_mutex_t lock;

int main()
{

	pthread_t suma, resta;
	pthread_mutex_init(&lock, NULL);
	
	// Crea el hilo que suma
	pthread_create(&suma, NULL, &incrementa, NULL);
	
	// Crea el hilo que resta
	pthread_create(&resta, NULL, &decrementa, NULL);
	
	// Espera a que ambos hilos terminen
	pthread_join(resta, NULL);
	pthread_join(suma, NULL);

	printf("El valor final de la variable es %d\n", global_var);
	pthread_mutex_destroy(&lock);
	
	return 0;
}

// Función que ejecutará el hilo de suma
void *incrementa()
{
	puts("Comienza hilo suma\n");
	
	for (int i=0;i<LIMITE;i++)
	{
		// Bloquea el mutex antes de acceder a la variable compartida
		pthread_mutex_lock(&lock);
		global_var=global_var+1;
		// Desbloquea el mutex después de modificar la variable
		pthread_mutex_unlock(&lock);
	}
	
	return 0;
}

// Función que ejecutará el hilo de resta
void *decrementa()
{
	puts("Comienza hilo resta\n");
	
	for (int i=0;i<LIMITE;i++)
	{
		// Bloquea el mutex antes de acceder a la variable compartida
		pthread_mutex_lock(&lock);
		global_var=global_var-1;
		// Desbloquea el mutex después de modificar la variable
		pthread_mutex_unlock(&lock);
	}
	
	return 0;
}