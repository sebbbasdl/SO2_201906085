#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHAR_PER_LINE 150
#define MAX_FIELD_SIZE 50
#define MAX_RECORDS 250

struct Usuario {
    int no_cuenta;
    char nombre[MAX_FIELD_SIZE];
    float saldo;
};

struct Transacciones {
    int operacion;
    int cuenta1;
    int cuenta2;
    float monto; 
};

bool existeCuenta(int num_cuenta, struct Usuario usuarios[MAX_RECORDS], int num_usuarios, int line_number) {
    for (int i = 0; i < num_usuarios; i++) {
        if (num_cuenta == usuarios[i].no_cuenta) {
            return true;
        }
    }
    printf("Error en línea %d: No se encontró la cuenta %d.\n", line_number, num_cuenta);
    return false;
}

bool validaciones(char linea[MAX_CHAR_PER_LINE], struct Usuario usuarios[MAX_RECORDS], int num_usuarios, struct Transacciones transacciones[MAX_RECORDS], int num_transacciones, int line_number) {
    char *token;
    //operacion;;
    
    token = strtok(linea, ",");
    int ope = atoi(token);

    transacciones[num_transacciones].operacion= ope;

    token = strtok(NULL, ",");
    
    if (token == NULL) {
        printf("Error en línea %d: No se pudo leer la cuenta1.\n", line_number);
        return false;
    }
    int cuenta1 = atoi(token);
    if (!existeCuenta(cuenta1, usuarios, num_usuarios, line_number)) {
        return false;
    }
    transacciones[num_transacciones].cuenta1=cuenta1;

    token = strtok(NULL, ",");
    if (token == NULL) {
        printf("Error en línea %d: No se pudo leer la cuenta2.\n", line_number);
        return false;
    }
    int cuenta2 = atoi(token);
    if(ope==2 || ope==1 ){
        
        if(cuenta2!=0){
            printf("Error en línea %d: En la operación (%d) el parámetro cuenta2 debe ser 0.\n", line_number, ope);
            return false;
        }

    }else if (ope==3 && !existeCuenta(cuenta1, usuarios, num_usuarios, line_number)) {
        return false;
    }
    transacciones[num_transacciones].cuenta2=cuenta2;

    token = strtok(NULL, ",");
    if (token == NULL) {
        printf("Error en línea %d: No se pudo leer el monto.\n", line_number);
        return false;
    }
    float monto = atof(token);
    if (monto < 0) {
        printf("Error en línea %d: El saldo para la transacción %d entre la cuenta1 %d y la cuenta2 %d es negativo.\n", line_number, transacciones[num_transacciones].operacion, transacciones[num_transacciones].cuenta1, transacciones[num_transacciones].cuenta2);
        return false;
    }
    transacciones[num_transacciones].monto=monto;

    return true;
}

void transacciones(struct Usuario usuarios[MAX_RECORDS], int num_usuarios) {
    FILE *archivo;
    char linea[MAX_CHAR_PER_LINE];
    struct Transacciones transacciones[MAX_RECORDS];
    int num_transacciones = 0;

    archivo = fopen("prueba_transacciones.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de transacciones");
        return;
    }
    // Leer y descartar la primera línea
    char primera_linea[MAX_CHAR_PER_LINE];
    if (fgets(primera_linea, sizeof(primera_linea), archivo) == NULL) {
        printf("Error al leer la primera línea del archivo de transacciones.\n");
        fclose(archivo);
        return;
    }

    int line_number = 1; // Contador de líneas

    while (fgets(linea, sizeof(linea), archivo)) {
        line_number++;
        char aux_linea[MAX_CHAR_PER_LINE];
        strcpy(aux_linea, linea);
        
        char *token;
        token = strtok(linea, ",");
        if (token == NULL) {
            printf("Error en línea %d: No se pudo leer la operación.\n", line_number);
            continue;
        }
        int operacion = atoi(token);
        
        if (operacion == 1 && validaciones(aux_linea,usuarios,num_usuarios,transacciones,num_transacciones, line_number)==true) {
            
            
        } else if (operacion == 2 && validaciones(aux_linea,usuarios,num_usuarios,transacciones,num_transacciones,line_number)==true) {
            
        } else if (operacion == 3 && validaciones(aux_linea,usuarios,num_usuarios,transacciones,num_transacciones,line_number)==true) {
            
        } else {
            printf("Error en linea %d: Operacion no valida.\n", line_number);
            continue;
        }
        num_transacciones++;
    }
    printf("Número de transacciones: %d\n", num_transacciones);
    fclose(archivo);
}

int main() {
    FILE *archivo;
    char linea[MAX_CHAR_PER_LINE];
    struct Usuario usuarios[MAX_RECORDS];
    int num_usuarios = 0;

    archivo = fopen("prueba_usuarios.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de usuarios");
        return 1;
    }

    // Leer y descartar la primera línea
    char primera_linea[MAX_CHAR_PER_LINE];
    if (fgets(primera_linea, sizeof(primera_linea), archivo) == NULL) {
        printf("Error al leer la primera línea del archivo de usuarios.\n");
        fclose(archivo);
        return 0;
    }

    int line_number = 1; // Contador de líneas

    while (fgets(linea, sizeof(linea), archivo)) {
        line_number++;
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
        float saldo = atof(token);
        if (saldo < 0) {
            printf("Error en línea %d: El saldo para el usuario %s es negativo.\n", line_number, usuarios[num_usuarios].nombre);
            continue;
        }
        usuarios[num_usuarios].saldo = saldo;

        num_usuarios++;
        
    }

    fclose(archivo);

    printf("Usuarios leídos:\n");
    for (int i = 0; i < num_usuarios; i++) {
        printf("Número de cuenta: %d, Nombre: %s, Saldo: %.2f\n", usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }

    printf("--------EMPIEZA TRANSACCIONES--------\n");
    transacciones(usuarios, num_usuarios);

    return 0;
}
