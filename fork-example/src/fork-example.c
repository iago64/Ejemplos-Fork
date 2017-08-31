#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <commons/collections/list.h>

//Defino una variable para saber si tengo que checkear si estan vivos todos los hijos:
bool bajas;

void chldKilled(int signum){
	puts("procesosHijos = procesosHijos - 1\n");
	bajas = true;
}

t_list * hijos;
typedef struct datosHijo{
	char *nombre;
	int pid;
}t_hijo;

void chld_destroy(t_hijo* self){
	printf("La palmo %s, con el PID: %d\n", self->nombre, self->pid);
	free(self->nombre);
	free(self);
}

void controlDeCabezas(void * cabeza){
	t_hijo* cabezaHijo = cabeza;
	if(0 != kill(cabezaHijo->pid, 0)){
		chld_destroy(cabeza);
	}
}

t_hijo * chld_create(char* name, int pid){
	t_hijo *elWachin = malloc(sizeof(t_hijo));
	elWachin->nombre = strdup(name);
	elWachin->pid = pid;
	return elWachin;
}

void ejecucionHijo(int pid){
	while(pid == 0){
		printf("PID: %d, El ATR que me trajo al mundo es: %d \n", getpid(), getppid());
		sleep(2);
	}
}

void ejecucionPadre(int pidHijo){
	printf("Creado un wachin nuevo--> Documento: %d \n", pidHijo);
	list_add(hijos, chld_create("El Brayan", pidHijo));
	sleep(4);
}

int main(void){

	//Defino el manejador de la señal
	signal(SIGCHLD, chldKilled);

	//Creo la lista
	hijos = list_create();

	//Seteo el control de los procesos hijos
	bajas = false;

	//Seteo el pid del hijo para no generarlos de forma recursiva
	int pidHijo = 0;

	for(int i = 0; i < 10; i++){
		pidHijo = fork();
		ejecucionHijo(pidHijo);
		ejecucionPadre(pidHijo);
	}

	while(pidHijo != 0){
		sleep(10);
	}

	return EXIT_SUCCESS;
}
