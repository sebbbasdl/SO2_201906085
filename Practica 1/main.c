#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>


void main(){
    int numero_proceso;
    srand(time(NULL));

    // Generar un número aleatorio en el rango de 1 a 3
    numero_proceso = rand() % 3 + 1;

    printf("El número aleatorio del proceso seleccionado entre 1 y 3 es: %d\n", numero_proceso);

    int pid1, pid2, pid3;

    if(numero_proceso== 1)

    pid_t pid = fork(); //Se realiza el fork
    
    if(pid == -1){
        perror("fork");
        exit(1);
    }
    
    if(pid == 0){

        printf("Soy proceso hijo");
    
    }else{
        printf("Soy el proceso padre\n");

        /*Se espera a que el proceso hijo termine*/
        int status;
        wait(&status);

        /*Se imprime el codigo de salida del proceso hijo*/
        if(WIFEXITED(status)){
            printf("\nEl proceso hijo termino con estado: %d\n", WIFEXITED(status));
        } else {
            printf("Ocurrio un error al terminar el proceso hijo");
        }

        printf("Terminando proceso padre\n");
    }
}