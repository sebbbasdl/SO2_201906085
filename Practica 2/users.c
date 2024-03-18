#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h> // Librería para hilos
#include <time.h> // Librería para obtener la fecha y hora actual

#define MAX_CHAR_PER_LINE 150
#define MAX_FIELD_SIZE 50
#define MAX_RECORDS 250

struct Usuario {
    int no_cuenta;
    char nombre[MAX_FIELD_SIZE];
    float saldo;
};

typedef struct{
    pthread_mutex_t m;
    FILE* f;
    int* curr_line;
}mtex;

typedef struct{
    mtex* mutex;
    short thread_n;
    int* counter;
    int users_loaded;
}args;

bool existeCuenta(int num_cuenta, struct Usuario usuarios[MAX_RECORDS], int num_usuarios, int line_number) {
    for (int i = 0; i < num_usuarios; i++) {
        if (num_cuenta == usuarios[i].no_cuenta) {
            return true;
        }
    }
    printf("Error en línea %d: No se encontró la cuenta %d.\n", line_number, num_cuenta);
    return false;
}

void *load_users(void *arguments) {
    args *thread_args = (args*)arguments;
    mtex *mutex = thread_args->mutex;
    FILE *file = mutex->f;
    int *current_line = mutex->curr_line;
    int *counter = thread_args->counter;
    int *users_loaded = &(thread_args->users_loaded);

    // Ignorar la primera línea
    if (*current_line == 0) {
        char primera_linea[MAX_CHAR_PER_LINE];
        fgets(primera_linea, sizeof(primera_linea), file);
        (*current_line)++;
    }

    char linea[MAX_CHAR_PER_LINE];
    struct Usuario usuarios[MAX_RECORDS];
    int num_usuarios = 0;

    while (1) {
        pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
        int line_number = 1+(*current_line)++;
        pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

        if (fgets(linea, sizeof(linea), file) == NULL) {
            break; // Si no hay más líneas, terminar el hilo
        }

        char *token;
        token = strtok(linea, ",");
        int num_cuenta = atoi(token);
        int cuenta_existente = 0;
        for (int i = 0; i < num_usuarios; i++) {
            if (num_cuenta == usuarios[i].no_cuenta) {
                cuenta_existente = 1;
                break;
            }
        }
        if (cuenta_existente) {
            printf("Error en línea %d: El número de cuenta %d ya existe en el sistema.\n", line_number, num_cuenta);
            continue;
        }
        usuarios[num_usuarios].no_cuenta = num_cuenta;

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Error en línea %d: No se pudo leer el nombre.\n", line_number);
            continue;
        }
        strcpy(usuarios[num_usuarios].nombre, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Error en línea %d: No se pudo leer el saldo.\n", line_number);
            continue;
        }

        char *endptr;
        float saldo = strtof(token, &endptr);
        if (*endptr != '\0' && !isspace(*endptr)) {
            printf("Error en línea %d: El saldo para el usuario %s no es un número válido.\n", line_number, usuarios[num_usuarios].nombre);
            continue;
        }

        // Verificación de saldo negativo
        if (saldo < 0) {
            printf("Error en línea %d: El saldo para el usuario %s es negativo.\n", line_number, usuarios[num_usuarios].nombre);
            continue;
        }

        usuarios[num_usuarios].saldo = saldo;

        num_usuarios++;
    }

    *users_loaded = num_usuarios; // Guardar el número de usuarios cargados por este hilo

    pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
    (*counter)++; // Aumentar el contador de hilos terminados
    pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

    pthread_exit(NULL); // Salir del hilo
}

int main() {
    FILE *archivo;
    archivo = fopen("prueba_usuarios.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de usuarios");
        return 1;
    }

    mtex mutex;
    pthread_mutex_init(&mutex.m, NULL); // Inicializar mutex
    mutex.f = archivo;
    int current_line = 0;
    mutex.curr_line = &current_line;

    pthread_t threads[3];
    args arguments[3];

    int counter = 0;
    int total_users_loaded = 0; // Contador total de usuarios cargados

    for (int i = 0; i < 3; i++) {
        arguments[i].mutex = &mutex;
        arguments[i].thread_n = i;
        arguments[i].counter = &counter;
        arguments[i].users_loaded = 0; // Inicializar el contador de usuarios cargados por este hilo
        pthread_create(&threads[i], NULL, load_users, (void *)&arguments[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL); // Esperar a que los hilos terminen
        total_users_loaded += arguments[i].users_loaded; // Sumar al total de usuarios cargados
    }

    pthread_mutex_destroy(&mutex.m); // Destruir mutex

    fclose(archivo);

    // Generar reporte de carga
    time_t current_time;
    char report_filename[50];
    struct tm *time_info;
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(report_filename, sizeof(report_filename), "carga_%Y_%m_%d-%H_%M_%S.log", time_info);

    FILE *report_file = fopen(report_filename, "w");
    if (report_file == NULL) {
        perror("Error al crear el archivo de reporte");
        return 1;
    }

    fprintf(report_file, "Reporte de carga de usuarios generado el %s\n\n", report_filename);
    fprintf(report_file, "Desglose de usuarios cargados por cada hilo:\n");
    for (int i = 0; i < 3; i++) {
        fprintf(report_file, "Hilo %d: %d usuarios\n", i + 1, arguments[i].users_loaded);
    }
    fprintf(report_file, "\nTotal de usuarios cargados: %d\n", total_users_loaded);

    fclose(report_file);

    printf("Reporte de carga generado correctamente en %s.\n", report_filename);

    return 0;
}
