#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>


void main(){
    

    int pid1, pid2, pid3;
    int count = 10;
    while(1){
        int numero_proceso;
        srand(time(NULL));

        // Generar un número aleatorio en el rango de 1 a 3
        numero_proceso = rand() % 3 + 1;        
        printf("El número aleatorio del proceso seleccionado entre 1 y 3 es: %d\n", numero_proceso);
        
        int espera;
        srand(time(NULL));
        espera = rand() % 3 + 1;
        printf("El tiempo de espera es de : %d\n", espera) ;
        sleep(espera);

        if(numero_proceso== 1){
            pid1=fork();
            if(pid1 == 0){
                printf("child[1] --> pid = %d and ppid = %d\n",getpid(), getppid());
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

        }else if (numero_proceso== 2) {
            pid2 = fork();
            if(pid2 == 0){
                printf("child[2] --> pid = %d and ppid = %d\n",getpid(), getppid());
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
        }else if (numero_proceso== 3) {
            pid3 = fork();
            if(pid3 == 0){
                printf("child[3] --> pid = %d and ppid = %d\n",getpid(), getppid());
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

    }


}