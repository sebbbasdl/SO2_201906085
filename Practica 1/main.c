#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <signal.h>

int contar_palabra(const char *archivo, const char *palabra) {
    FILE *file;
    char line[256];
    int contador = 0;

    file = fopen(archivo, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, palabra) != NULL) {
            contador++;
        }
    }

    fclose(file);

    return contador;
}

// Función de manejo de la señal SIGINT
void sigint_handler(int signal) {
    printf("Se recibió la señal SIGINT (Ctrl+C).\n");
    const char *archivo = "syscalls.log";
    printf("Cantidad de procesos: %d\n", contar_palabra(archivo, "proceso"));
    printf("Read: %d, Write: %d\n", contar_palabra(archivo, "read"),contar_palabra(archivo, "write"));
    exit(0); // Por ejemplo, puedes salir del programa
}



int main() {
    int n = 2; // crear 2 hijos
    int i = 0;
    int status = 0;
    int child_count = 0; // Contador para el número de hijos creados
    char command[100];

    // Establecer el manejador de señales para SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error al establecer el manejador de señales");
        return 1;
    }
    // Crear o vaciar el archivo practica1.txt
    FILE *file = fopen("practica1.txt", "w");
    if (file == NULL) {
        perror("Error al abrir el archivo practica1.txt");
        return 1;
    }
    fclose(file);
    

    pid_t child_pids[n]; // Arreglo para almacenar los PID de los hijos

    for (i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {
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
                //printf("Soy los hijosss con PID: %d %s %d\n", child_pids[0],"   " ,child_pids[1]);
                sprintf(command, "%s %d %d %s", "sudo stap trace.stp ", child_pids[0], child_pids[1], " > syscalls.log");
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
