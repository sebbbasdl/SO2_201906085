#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char command[100];
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int pid1 = atoi(argv[0]);
    printf("Soy el PID: %d\n", pid1);

    /*sprintf(command, "%s %d  %s", "sudo stap trace.stp ", pid1, " > calls.log");
    system(command);*/
    FILE *archivo;
    int opcion;
    char buffer[9]; // Buffer para almacenar los primeros 8 caracteres y el terminador nulo
    
    // Abrir el archivo
    archivo = fopen("practica1.txt", "a+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    }

    while(1) {
        srand(time(NULL));
        
        // Generar número aleatorio del 1 al 3
        opcion = rand() % 3 + 1;

        // Caso 1: Escribir en el archivo
        if (opcion == 1) {
            fprintf(archivo, "escribir\n");
            fflush(archivo); // Forzar la escritura inmediata en el archivo
            printf("Se escribió 'escribir' en el archivo practica1.txt\n");
        } 
        // Caso 2: Leer los primeros 8 caracteres del archivo
        else if (opcion == 2) {
            printf("Primeros 8 caracteres del archivo practica1.txt: ");
            fseek(archivo, 0, SEEK_SET);
            if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
                printf("%s\n", buffer);
            } else {
                printf("El archivo está vacío o no se pudo leer.\n");
            }
        } 
        // Caso 3: Posicionar el puntero de archivo al inicio
        else if (opcion == 3) {
            printf("Posicionando puntero al inicio del archivo.\n");
            fseek(archivo, 0, SEEK_SET);
        }

        int espera = rand() % 3 + 1;
        sleep(espera);
        


    }

    // Cerrar el archivo después de salir del bucle
    fclose(archivo);
    return 0;
}
