#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h> // Librería para hilos
#include <time.h>    // Librería para obtener la fecha y hora actual

#define MAX_CHAR_PER_LINE 150
#define MAX_FIELD_SIZE 50
#define MAX_RECORDS 250

struct Usuario
{
    int no_cuenta;
    char nombre[MAX_FIELD_SIZE];
    float saldo;
};

struct Transaccion
{
    int operacion;
    int cuenta1;
    int cuenta2;
    float monto;
};

typedef struct
{
    pthread_mutex_t m;
    FILE *f;
    int *curr_line;
    char *errors; // Cadena para almacenar los errores
} Mutex;

typedef struct {
    Mutex* mutex;
    struct Usuario* usuarios; // Puntero al arreglo de usuarios
    short thread_n;
    int* counter;
    int transactions_loaded;
    char *errors; // Cadena para almacenar los errores
} ThreadArgs;


void deposito(struct Usuario usuarios[MAX_RECORDS], int num_usuarios) {
    int cuenta;
    float monto;
    printf("\nIngrese el número de cuenta para el depósito: ");
    scanf("%d", &cuenta);
    printf("Ingrese el monto a depositar: ");
    scanf("%f", &monto);

    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].no_cuenta == cuenta) {
            usuarios[i].saldo += monto;
            printf("Depósito realizado con éxito. Nuevo saldo para la cuenta %d: %.2f\n", cuenta, usuarios[i].saldo);
            return;
        }
    }
    printf("La cuenta ingresada no fue encontrada.\n");
}

void retiro(struct Usuario usuarios[MAX_RECORDS], int num_usuarios) {
    int cuenta;
    float monto;
    printf("\nIngrese el número de cuenta para el retiro: ");
    scanf("%d", &cuenta);
    printf("Ingrese el monto a retirar: ");
    scanf("%f", &monto);

    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].no_cuenta == cuenta) {
            if (usuarios[i].saldo >= monto) {
                usuarios[i].saldo -= monto;
                printf("Retiro realizado con éxito. Nuevo saldo para la cuenta %d: %.2f\n", cuenta, usuarios[i].saldo);
            } else {
                printf("Saldo insuficiente para realizar el retiro.\n");
            }
            return;
        }
    }
    printf("La cuenta ingresada no fue encontrada.\n");
}

void transferencia(struct Usuario usuarios[MAX_RECORDS], int num_usuarios) {
    int cuenta_origen, cuenta_destino;
    float monto;
    printf("\nIngrese el número de cuenta de origen: ");
    scanf("%d", &cuenta_origen);
    printf("Ingrese el número de cuenta de destino: ");
    scanf("%d", &cuenta_destino);
    printf("Ingrese el monto a transferir: ");
    scanf("%f", &monto);

    int encontrado_origen = 0, encontrado_destino = 0;
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].no_cuenta == cuenta_origen) {
            encontrado_origen = 1;
            if (usuarios[i].saldo >= monto) {
                usuarios[i].saldo -= monto;
                for (int j = 0; j < num_usuarios; j++) {
                    if (usuarios[j].no_cuenta == cuenta_destino) {
                        encontrado_destino = 1;
                        usuarios[j].saldo += monto;
                        printf("Transferencia realizada con éxito de la cuenta %d a la cuenta %d\n", cuenta_origen, cuenta_destino);
                        return;
                    }
                }
            } else {
                printf("Saldo insuficiente en la cuenta de origen.\n");
                return;
            }
        }
    }
    if (!encontrado_origen) {
        printf("La cuenta de origen ingresada no fue encontrada.\n");
    }
    if (!encontrado_destino) {
        printf("La cuenta de destino ingresada no fue encontrada.\n");
    }
}

void consultar_cuenta(struct Usuario usuarios[MAX_RECORDS], int num_usuarios) {
    int cuenta;
    printf("\nIngrese el número de cuenta a consultar: ");
    scanf("%d", &cuenta);

    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].no_cuenta == cuenta) {
            printf("Información de la cuenta %d:\n", cuenta);
            printf("Nombre: %s\n", usuarios[i].nombre);
            printf("Saldo: %.2f\n", usuarios[i].saldo);
            return;
        }
    }
    printf("La cuenta ingresada no fue encontrada.\n");
}

bool existeCuenta(int num_cuenta, struct Usuario usuarios[MAX_RECORDS], int num_usuarios, int line_number)
{
    for (int i = 0; i < num_usuarios; i++)
    {
        if (num_cuenta == usuarios[i].no_cuenta)
        {
            return true;
        }
    }
    printf("Error en línea %d: No se encontró la cuenta %d.\n", line_number, num_cuenta);
    return false;
}

float validaciones(char linea[MAX_CHAR_PER_LINE], struct Usuario usuarios[MAX_RECORDS], int num_usuarios, struct Transaccion transacciones[MAX_RECORDS], int num_transacciones, int line_number)
{
    char *token;
    //operacion;;

    token = strtok(linea, ",");
    int ope = atoi(token);

    transacciones[num_transacciones].operacion = ope;

    token = strtok(NULL, ",");

    if (token == NULL)
    {
        printf("Error en línea %d: No se pudo leer la cuenta1.\n", line_number);
        return -999;
    }
    
    int cuenta1 = atoi(token);

    if (!existeCuenta(cuenta1, usuarios, num_usuarios, line_number))
    {

        return -9999;
    }
    transacciones[num_transacciones].cuenta1 = cuenta1;

    token = strtok(NULL, ",");
    if (token == NULL)
    {
        printf("Error en línea %d: No se pudo leer la cuenta2.\n", line_number);
        return -999;
    }
    
    int cuenta2 = atoi(token);
    if (ope == 2 || ope == 1)
    {

        if (cuenta2 != 0)
        {
            printf("Error en línea %d: En la operación (%d) el parámetro cuenta2 debe ser 0.\n", line_number, ope);
            return -99999;
        }
    }
    else if (ope == 3 && !existeCuenta(cuenta2, usuarios, num_usuarios, line_number))
    {
        return -9999;
    }
    transacciones[num_transacciones].cuenta2 = cuenta2;

    token = strtok(NULL, ",");
    
    if (token == NULL)
    {
        printf("Error en línea %d: No se pudo leer el monto.\n", line_number);
        return -999;
    }
    char *endptr;
    float monto = strtof(token, &endptr);
    if (*endptr != '\0' && !isspace(*endptr))
    {
        printf("Error en línea %d: El monto para el usuario %s no es un número válido.\n", line_number, usuarios[num_usuarios].nombre);
        return -999999;
    }

    // Verificación de saldo negativo
    if (monto < 0)
    {
        printf("Error en línea %d: El monto para el usuario %s es negativo.\n", line_number, usuarios[num_usuarios].nombre);
        return -9999999;
    }
    transacciones[num_transacciones].monto = monto;

    return monto;
}

void *load_users(void *arguments)
{
    ThreadArgs *thread_args = (ThreadArgs *)arguments;
    Mutex *mutex = thread_args->mutex;
    FILE *file = mutex->f;
    int *current_line = mutex->curr_line;
    int *counter = thread_args->counter;
    int *users_loaded = &(thread_args->transactions_loaded);
    struct Usuario *usuarios = thread_args->usuarios; 
    char *errors = thread_args->errors; // Cadena para almacenar los errores

    // Ignorar la primera línea
    if (*current_line == 0)
    {
        char primera_linea[MAX_CHAR_PER_LINE];
        fgets(primera_linea, sizeof(primera_linea), file);
        (*current_line)++;
    }

    char linea[MAX_CHAR_PER_LINE];
    int num_usuarios = 0;

    while (1)
    {
        pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
        int line_number = 1 + (*current_line)++;
        pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

        if (fgets(linea, sizeof(linea), file) == NULL)
        {
            break; // Si no hay más líneas, terminar el hilo
        }

        char *token;
        token = strtok(linea, ",");
        int num_cuenta = atoi(token);
        int cuenta_existente = 0;
        for (int i = 0; i < num_usuarios; i++)
        {
            if (num_cuenta == usuarios[i].no_cuenta)
            {
                cuenta_existente = 1;
                break;
            }
        }
        if (cuenta_existente)
        {
            sprintf(errors + strlen(errors), "Error en línea %d: El número de cuenta %d ya existe en el sistema.\n", line_number, num_cuenta);
            continue;
        }
        usuarios[num_usuarios].no_cuenta = num_cuenta;

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            sprintf(errors + strlen(errors), "Error en línea %d: No se pudo leer el nombre.\n", line_number);
            continue;
        }
        strcpy(usuarios[num_usuarios].nombre, token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            sprintf(errors + strlen(errors), "Error en línea %d: No se pudo leer el saldo.\n", line_number);
            continue;
        }

        char *endptr;
        float saldo = strtof(token, &endptr);
        if (*endptr != '\0' && !isspace(*endptr))
        {
            sprintf(errors + strlen(errors), "Error en línea %d: El saldo para el usuario %s no es un número válido.\n", line_number, usuarios[num_usuarios].nombre);
            continue;
        }

        // Verificación de saldo negativo
        if (saldo < 0)
        {
            sprintf(errors + strlen(errors), "Error en línea %d: El saldo para el usuario %s es negativo.\n", line_number, usuarios[num_usuarios].nombre);
            continue;
        }

        usuarios[num_usuarios].saldo = saldo;

        num_usuarios++;
    }

    *users_loaded = num_usuarios; // Guardar el número de usuarios cargados por este hilo

    pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
    (*counter)++;                   // Aumentar el contador de hilos terminados
    pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

    pthread_exit(NULL); // Salir del hilo
}

void *load_transactions(void *arguments)
{
    ThreadArgs *thread_args = (ThreadArgs *)arguments;
    Mutex *mutex = thread_args->mutex;
    FILE *file = mutex->f;
    int *current_line = mutex->curr_line;
    int *counter = thread_args->counter;
    int *transactions_loaded = &(thread_args->transactions_loaded);
    struct Transaccion transacciones[MAX_RECORDS];
    int num_transacciones = 0;
    struct Usuario *usuarios = thread_args->usuarios;
    char *errors = thread_args->errors; // Cadena para almacenar los errores

    // Ignorar la primera línea
    if (*current_line == 0)
    {
        char primera_linea[MAX_CHAR_PER_LINE];
        fgets(primera_linea, sizeof(primera_linea), file);
        (*current_line)++;
    }

    char linea[MAX_CHAR_PER_LINE];
    int num_usuarios = 94; // Utilizar el número de usuarios cargados
    
    // Imprimir todos los usuarios
    /*printf("Usuarios cargados:\n");
    for (int i = 0; i < num_usuarios; i++) {
        printf("Cuenta: %d, Nombre: %s, Saldo: %.2f\n", usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }
    printf("\n");*/

    while (1)
    {
        pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
        int line_number = 1 + (*current_line)++;
        pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

        if (fgets(linea, sizeof(linea), file) == NULL)
        {
            break; // Si no hay más líneas, terminar el hilo
        }

        
        char aux_linea[MAX_CHAR_PER_LINE];
        strcpy(aux_linea, linea);
        
        char *token;
        token = strtok(linea, ",");
        
        if (token == NULL)
        {
            sprintf(errors + strlen(errors), "Error en línea %d: No se pudo leer la operación.\n", line_number);
            continue;
        }
        
        int operacion = atoi(token);

        if (operacion == 1 || operacion == 2 || operacion == 3)
        {
            float monto = validaciones(aux_linea, usuarios, num_usuarios, transacciones, num_transacciones, line_number);
            if (monto >= -999 )
            {
                if (operacion == 1)
                {
                    for (int i = 0; i < num_usuarios; i++)
                    {
                        if (usuarios[i].no_cuenta == transacciones[num_transacciones].cuenta1)
                        {
                            usuarios[i].saldo += monto;
                        }
                    }
                }
                else if (operacion == 2)
                {
                    for (int i = 0; i < num_usuarios; i++)
                    {
                        if (usuarios[i].no_cuenta == transacciones[num_transacciones].cuenta1)
                        {
                            int saldo_anterior = usuarios[i].saldo;
                            usuarios[i].saldo = usuarios[i].saldo - monto;
                            if (usuarios[i].saldo < 0)
                            {
                                usuarios[i].saldo = saldo_anterior;
                                sprintf(errors + strlen(errors), "Error en línea %d: No se puede retirar esa cantidad ya que es mayor a su saldo.\n", line_number);
                                break;
                            }
                        }
                    }
                }
                else if (operacion == 3)
                {
                    int pos_c1;
                    int pos_c2;

                    for (int i = 0; i < num_usuarios; i++)
                    {
                        if (usuarios[i].no_cuenta == transacciones[num_transacciones].cuenta1)
                        {
                            pos_c1 = i;
                        }
                        if (usuarios[i].no_cuenta == transacciones[num_transacciones].cuenta2)
                        {
                            pos_c2 = i;
                        }
                    }

                    if (usuarios[pos_c1].no_cuenta == usuarios[pos_c2].no_cuenta)
                    {
                        sprintf(errors + strlen(errors), "Error en línea %d: No se puede transferir ya que cuenta 1 y cuenta 2 son el mismo número de cuenta.\n", line_number);
                        continue;
                    }

                    int saldo_anterior_c1 = usuarios[pos_c1].saldo;
                    usuarios[pos_c1].saldo = usuarios[pos_c1].saldo - monto;

                    if (usuarios[pos_c1].saldo < 0)
                    {
                        usuarios[pos_c1].saldo = saldo_anterior_c1;
                        sprintf(errors + strlen(errors), "Error en línea %d: No se puede transferir esa cantidad ya que es mayor a saldo.\n", line_number);
                        continue;
                    }
                    else
                    {
                        usuarios[pos_c2].saldo += monto;
                    }
                }
            }else{
                
                if(monto == -9999){
                
                    sprintf(errors + strlen(errors), "Error en línea %d: No existe cuenta.\n", line_number);
                }else if(monto == -99999){
                
                    sprintf(errors + strlen(errors), "Error en línea %d: En la operación (%d) el parámetro cuenta2 debe ser 0.\n", line_number, operacion);
                }else if(monto == -999999){
                
                    sprintf(errors + strlen(errors), "Error en línea %d: El monto no es un numero valido\n", line_number);
                }else if(monto == -9999999){
                
                    sprintf(errors + strlen(errors), "Error en línea %d: El monto es negativo\n", line_number);
                }
                continue;
            }
            num_transacciones++;
        }
        else
        {
            sprintf(errors + strlen(errors), "Error en línea %d: Operación no válida.\n", line_number);
        }
    }
    *transactions_loaded = num_transacciones; // Guardar el número de transacciones cargados por este hilo
    pthread_mutex_lock(&mutex->m); // Bloquear sección crítica
    (*counter)++;                   // Aumentar el contador de hilos terminados
    pthread_mutex_unlock(&mutex->m); // Desbloquear sección crítica

    pthread_exit(NULL); // Salir del hilo
}

int main()
{
    char usuarios_filename[100]; // Variable para almacenar la ruta del archivo de usuarios
    char transacciones_filename[100]; // Variable para almacenar la ruta del archivo de transacciones

    printf("Ingrese la ruta del archivo de usuarios: ");
    scanf("%s", usuarios_filename); // Leer la ruta del archivo de usuarios

    printf("Ingrese la ruta del archivo de transacciones: ");
    scanf("%s", transacciones_filename); // Leer la ruta del archivo de transacciones

    FILE *usuarios_file, *transacciones_file;
    usuarios_file = fopen(usuarios_filename, "r"); // Abrir el archivo de usuarios
    transacciones_file = fopen(transacciones_filename, "r"); // Abrir el archivo de transacciones
    if (usuarios_file == NULL || transacciones_file == NULL)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    Mutex usuarios_mutex, transacciones_mutex;
    pthread_mutex_init(&usuarios_mutex.m, NULL);     // Inicializar mutex para usuarios
    pthread_mutex_init(&transacciones_mutex.m, NULL); // Inicializar mutex para transacciones
    usuarios_mutex.f = usuarios_file;
    transacciones_mutex.f = transacciones_file;
    int current_line_usuarios = 0, current_line_transacciones = 0;
    usuarios_mutex.curr_line = &current_line_usuarios;
    transacciones_mutex.curr_line = &current_line_transacciones;

    pthread_t threads[3];                             // Se crean 3 hilos para cargar usuarios
    ThreadArgs arguments_usuarios[3];                 // Argumentos para los hilos de carga de usuarios
    int counter_usuarios = 0;
    int total_users_loaded = 0;                       // Contador total de usuarios cargados
    struct Usuario usuarios[MAX_RECORDS];             // Arreglo de usuarios
    char usuarios_errors[5000] = ""; // Cadena para almacenar los errores de usuarios

    for (int i = 0; i < 3; i++)
    {
        arguments_usuarios[i].mutex = &usuarios_mutex;
        arguments_usuarios[i].thread_n = i;
        arguments_usuarios[i].counter = &counter_usuarios;
        arguments_usuarios[i].transactions_loaded = 0; // Inicializar el contador de usuarios cargados por este hilo
        arguments_usuarios[i].usuarios = usuarios;     // Pasar el arreglo de usuarios como argumento
        arguments_usuarios[i].errors = usuarios_errors; // Pasar la cadena de errores como argumento
        pthread_create(&threads[i], NULL, load_users, (void *)&arguments_usuarios[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);                // Esperar a que los hilos de usuarios terminen
        total_users_loaded += arguments_usuarios[i].transactions_loaded; // Sumar al total de usuarios cargados
    }

    pthread_mutex_destroy(&usuarios_mutex.m);          // Destruir mutex para usuarios

    printf("Total de usuarios cargados: %d\n", total_users_loaded);
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
        fprintf(report_file, "Hilo %d: %d usuarios\n", i + 1, arguments_usuarios[i].transactions_loaded);
    }
    fprintf(report_file, "\nTotal de usuarios cargados: %d\n", total_users_loaded);
    fprintf(report_file, "Errores durante la carga de usuarios:\n%s\n", usuarios_errors);

    fclose(report_file);

    printf("Reporte de carga generado correctamente en %s.\n", report_filename);

    

    // Ahora cargar transacciones

    pthread_t threads_transacciones[4];                // Se crean 4 hilos para cargar transacciones
    ThreadArgs arguments_transacciones[4];             // Argumentos para los hilos de carga de transacciones
    int counter_transacciones = 0;
    int total_transactions_loaded = 0;                 // Contador total de transacciones cargadas
    char transactions_errors[5000] = ""; // Cadena para almacenar los errores de transacciones
    
    for (int i = 0; i < 4; i++)
    {
        arguments_transacciones[i].mutex = &transacciones_mutex;
        arguments_transacciones[i].thread_n = i;
        arguments_transacciones[i].counter = &counter_transacciones;
        arguments_transacciones[i].transactions_loaded = total_transactions_loaded; // Inicializar el contador de transacciones cargadas por este hilo
        arguments_transacciones[i].usuarios = usuarios;     // Pasar el arreglo de usuarios como argumento
        arguments_transacciones[i].errors = transactions_errors; // Pasar la cadena de errores como argumento
        pthread_create(&threads_transacciones[i], NULL, load_transactions, (void *)&arguments_transacciones[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads_transacciones[i], NULL);                // Esperar a que los hilos de transacciones terminen
        total_transactions_loaded += arguments_transacciones[i].transactions_loaded; // Sumar al total de transacciones cargadas
    }

    pthread_mutex_destroy(&transacciones_mutex.m);          // Destruir mutex para transacciones

    printf("Total de transacciones cargadas: %d\n", total_transactions_loaded);
    char transactions_report_filename[50];
    strftime(transactions_report_filename, sizeof(transactions_report_filename), "transacciones_%Y_%m_%d-%H_%M_%S.log", time_info);

    FILE *transactions_report_file = fopen(transactions_report_filename, "w");
    if (transactions_report_file == NULL) {
        perror("Error al crear el archivo de reporte de transacciones");
        return 1;
    }

    fprintf(transactions_report_file, "Reporte de carga de transacciones generado el %s\n\n", transactions_report_filename);
    fprintf(transactions_report_file, "Desglose de transacciones cargadas por cada hilo:\n");
    for (int i = 0; i < 4; i++) {
        fprintf(transactions_report_file, "Hilo %d: %d transacciones\n", i + 1, arguments_transacciones[i].transactions_loaded);
    }
    fprintf(transactions_report_file, "\nTotal de transacciones cargadas: %d\n", total_transactions_loaded);
    fprintf(transactions_report_file, "Errores durante la carga de transacciones:\n%s\n", transactions_errors);

    fclose(transactions_report_file);

    printf("Reporte de carga de transacciones generado correctamente en %s.\n", transactions_report_filename);

    fclose(usuarios_file);
    fclose(transacciones_file);

    //MENU
    int opcion;
    do {
        printf("\nMenú de operaciones:\n");
        printf("1. Depósito\n");
        printf("2. Retiro\n");
        printf("3. Transferencia\n");
        printf("4. Consultar cuenta\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                deposito(usuarios, total_users_loaded);
                break;
            case 2:
                retiro(usuarios, total_users_loaded);
                break;
            case 3:
                transferencia(usuarios, total_users_loaded);
                break;
            case 4:
                consultar_cuenta(usuarios, total_users_loaded);
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Por favor, seleccione una opción válida.\n");
        }
    } while (opcion != 5);

    return 0;
}
