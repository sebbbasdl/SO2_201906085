#include <stdio.h>

int main() {
    FILE *archivo;
    char buffer[9]; // Buffer para almacenar los primeros 8 caracteres y el terminador nulo

    archivo = fopen("practica1.txt", "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    } else {
        // Leer los primeros 8 caracteres del archivo
        printf("Primeros 8 caracteres del archivo practica1.txt:  ");
        if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            printf("%s", buffer);
            printf("\n");
        } else {
            printf("El archivo está vacío o no se pudo leer.\n");
        }

        fclose(archivo);
    }

    return 0;
}
