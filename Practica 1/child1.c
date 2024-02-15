#include <stdio.h>

int main() {
    FILE *archivo;

    archivo = fopen("practica1.txt", "a");

    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    } else {
        fprintf(archivo, "escribir\n");
        printf("Se escribió 'escribir' en el archivo practica1.txt\n");
        fclose(archivo);
    }

    return 0;
}
