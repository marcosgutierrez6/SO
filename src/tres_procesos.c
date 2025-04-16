#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


static int *shared_num;
//esta variable contendrá el número compartido por los dos procesos
static int *global_flag;
//esta variable contiene un flag que utilizaremos para sincronizar los dos procesos
//-1 - No hay valor
// 1 - hay  valor
// 0 - Terminar


int main (void)
{
	pid_t lee, escribe;
	
	shared_num = mmap(NULL, sizeof *shared_num, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	global_flag = mmap(NULL, sizeof *global_flag, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	*global_flag=-1;
	
	lee=fork();
	if (lee>0) {
		escribe=fork();
	}
	if (lee==0)//estamos en el proceso lee
	{
		printf("introduce un número:\n");
		scanf("%d", &*shared_num);
		*global_flag=1;
		//cambiamos el valor del flag a 1, lo que singnifica que el ya hemos almacenado la variable. Ahora el proceso escribe
		//puede mostrarla por pantalla
		while(*global_flag!=0)
		{
			usleep(10);
		}
			
		puts ("Soy el proceso lee: me largo!");
		exit(1);
	}
	else if (escribe==0)// estamos en el proceso escribe
	{
		char c;
		while (*global_flag==-1)
		{
			usleep(10);
		}
		//Mientras el flag esté con valor -1 el proceso queda retenido en un bucle vacio -while();
		//Úna vez que el proceso 'lee' cambie el flag a 1 el proceso escribe podrá continuar
		printf("El usuario ha escrito: %d\n", *shared_num);
		puts("Pulse cualquier teclar para terminar");
		scanf(" %c", &c);
		puts ("Soy el proceso escribe: me largo!!");
		*global_flag=0;
		exit(1);
	}
	else{
		while ((wait (NULL))>0);//el proceso padre espera a que los hijos acaben
	}
	puts ("Soy el proceso padre: me largo!!!");
	return 0;
}

