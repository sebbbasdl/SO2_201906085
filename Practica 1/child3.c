#include <stdio.h>

int main() {
    FILE *archivo;

    archivo = fopen("practica1.txt", "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    } else {
        // Posicionar el puntero de archivo al inicio
        fseek(archivo, 0, SEEK_SET);

        fclose(archivo);
    }

    return 0;
}
