#include <stdio.h>
#include <stdlib.h>

int main(void) {
	while(1){
		printf("Genero el PID: %d\n", fork());

	}
	return EXIT_SUCCESS;
}
