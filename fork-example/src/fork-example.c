#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <commons/collections/list.h>

//Defino una variable para saber si tengo que checkear si estan vivos todos los hijos:
int bajas;
int cant_hijos;


t_list * hijos;
typedef struct datosHijo{
	char *nombre;
	int pid;
}t_hijo;

void chld_destroy(t_hijo* self){
	printf("Matamos a %s, con el PID: %d\n", self->nombre, self->pid);
	free(self->nombre);
	free(self);
}

void chldKilled(int signum){
	int status;
	printf("Vamos a matar al %d hijo\nSoy el padre: %d\n",bajas,getpid());
	t_hijo* hijo_a_matar;
	hijo_a_matar=list_get(hijos,bajas);
	printf("Chau %d\n",hijo_a_matar->pid);
	list_remove_and_destroy_element(hijos,bajas,(void*)chld_destroy);
	bajas++;
	kill(hijo_a_matar->pid,SIGKILL);
	waitpid(hijo_a_matar->pid,&status,0);
	cant_hijos--;
	printf("Ahora tengo %d hijos\n",cant_hijos);

}


t_hijo * chld_create(char* name, int pid){
	t_hijo *proceso_hijo = malloc(sizeof(t_hijo));
	proceso_hijo->nombre = strdup(name);
	proceso_hijo->pid = pid;
	return proceso_hijo;
}

void ejecucionHijo(int pid){
	while(pid == 0){
		printf("PID: %d, El proceso padre es: %d \n", getpid(), getppid());
		sleep(5);
	}
}

void ejecucionPadre(int pidHijo){
	printf("Creado un nuevo hijo--> PID: %d \n", pidHijo);
	list_add(hijos, chld_create("Jorge", pidHijo));
	//sleep(4);
}

int main(void){


	//Defino el manejador de la señal
	signal(SIGUSR1, chldKilled);

	//Creo la lista
	hijos = list_create();

	//Seteo el control de los procesos hijos
	bajas = 0;
	cant_hijos = 0;

	//Seteo el pid del hijo para no generarlos de forma recursiva
	int pidHijo = 0;
	int i;

	for(i = 0; i < 10; i++){
		cant_hijos++;
		pidHijo = fork();
		ejecucionHijo(pidHijo);
		ejecucionPadre(pidHijo);
	}

	int status;
	for(;;){
		sleep(10);
	}
}
