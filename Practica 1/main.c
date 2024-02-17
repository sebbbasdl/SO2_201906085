#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int foo(const char *whoami) {
    printf("I am a %s. My pid is: %d my ppid is %d\n",
           whoami, getpid(), getppid());
    return 1;
}

int main() {
    int n = 2; // crear 2 hijos
    int i = 0;
    int status = 0;
    int child_count = 0; // Contador para el número de hijos creados
    char command[100];

    foo("parent");

    pid_t child_pids[n]; // Arreglo para almacenar los PID de los hijos

    for (i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            foo("child");
            char pid_str[20]; // Suficientemente grande para contener el PID convertido a cadena
            snprintf(pid_str, sizeof(pid_str), "%d", getpid()); // Convierte el PID a cadena
            char *arg_Ptr[3];
            arg_Ptr[0] = pid_str;
            arg_Ptr[1] = "practica1.txt";
            arg_Ptr[2] = NULL;

            execv("/home/sebbbasdl/Documentos/SO2_201906085/Practica 1/child1.bin", arg_Ptr);

            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            // Proceso padre
            
            child_pids[child_count] = pid; // Almacena el PID del hijo en el arreglo
            child_count++; // Incrementa el contador de hijos creados
            

            if (child_count >= n) {
                printf("Soy los hijosss con PID: %d %s %d\n", child_pids[0],"   " ,child_pids[1]);
                sprintf(command, "%s %d %d %s", "sudo stap trace.stp ", child_pids[0], child_pids[1], " > calls.log");
                system(command);
                // Espera a que todos los hijos sean creados antes de continuar
                for (int j = 0; j < n; j++) {
                    waitpid(child_pids[j], &status, 0); // Espera específicamente al hijo con PID child_pids[j]
                }
                
                break; // Sal del bucle for después de que todos los hijos sean creados
            }
        } else {
            // Error al hacer fork
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
