Sebastian Alejandro de Leon Tenaz

201906085

Laboratorio de Sistemas Operativos 2 Sección A

## Manual Técnico - Práctica 2 -Multithreading

### Introducción

El presente manual técnico describe la arquitectura, funcionalidades y procedimientos de la aplicación bancaria desarrollada para gestionar la carga masiva de usuarios, así como operaciones monetarias como depósitos, retiros y transferencias. La aplicación está diseñada para ejecutarse en un entorno Linux y utiliza el lenguaje de programación C junto con conceptos de multiprocesamiento para garantizar la eficiencia y la integridad de los datos.

### Requisitos del Sistema

-   Sistema Operativo: Linux
-   Compilador de C compatible (ej. GCC)
-   Acceso a archivos CSV para carga masiva de usuarios y operaciones

### Estructura del Código

El código de la aplicación se divide en varias secciones principales:

1.  **Definición de Estructuras y Constantes:** Se definen las estructuras de datos necesarias para usuarios y transacciones, junto con constantes y bibliotecas requeridas.
    
2.  **Funciones de Validación y Procesamiento de Datos:** Se implementan funciones para validar y procesar datos de usuarios y operaciones, garantizando la integridad de los mismos.
    
3.  **Hilos y Sincronización:** Se definen y gestionan hilos para la carga masiva de usuarios y operaciones, utilizando técnicas de sincronización para evitar conflictos de datos.
    
4.  **Interfaz de Usuario y Funcionalidades:** Se proporciona una interfaz de consola para que los usuarios realicen operaciones como depósito, retiro, transferencia y consulta de cuentas.
    
5.  **Generación de Reportes:** Se generan reportes automáticos al finalizar la carga masiva y las operaciones, detallando los resultados y errores encontrados.
    

### Funcionamiento de la Aplicación

La aplicación sigue el siguiente flujo de trabajo:

1.  **Carga Masiva de Usuarios:**
    
    -   El usuario proporciona un archivo CSV con datos de usuarios.
    -   La aplicación carga los datos en 3 hilos, validando y procesando cada registro.
    -   Los errores encontrados se registran para su posterior análisis.
2.  **Operaciones Individuales:**
    
    -   Los usuarios pueden realizar depósitos, retiros, transferencias y consultas de cuenta desde la interfaz de consola.
    -   El sistema valida y ejecuta las operaciones, notificando errores si es necesario.
3.  **Carga Masiva de Operaciones:**
    
    -   Similar a la carga de usuarios, se proporciona un archivo CSV con operaciones monetarias.
    -   Las operaciones se realizan en 4 hilos, utilizando técnicas de sincronización para evitar conflictos.
4.  **Generación de Reportes:**
    
    -   Al finalizar la carga masiva de usuarios y operaciones, se generan reportes detallando los resultados y errores encontrados.

### Conclusiones

### Bibliotecas Utilizadas

La aplicación hace uso de las siguientes bibliotecas estándar de C:

1. **pthread.h:** Esta biblioteca proporciona las funciones y tipos necesarios para trabajar con hilos en C. Se utiliza para crear, gestionar y sincronizar hilos en la aplicación.

2. **stdio.h:** Esta biblioteca proporciona las funciones necesarias para entrada y salida estándar, como `printf` y `scanf`, utilizadas para interactuar con el usuario a través de la consola.

3. **stdlib.h:** Esta biblioteca estándar de C proporciona funciones para asignación de memoria dinámica, control de procesos y otras operaciones fundamentales.

4. **string.h:** La biblioteca `string.h` se utiliza para operaciones de manipulación de cadenas, como `strcpy` y `strtok`, que son necesarias para procesar datos de archivos CSV.

5. **stdbool.h:** Esta biblioteca define el tipo de datos `bool` y los valores `true` y `false`, utilizados para representar valores lógicos en C.

6. **ctype.h:** Esta biblioteca proporciona funciones para determinar el tipo de caracteres (como alfabéticos, numéricos, etc.) y realizar transformaciones de caracteres.

7. **time.h:** Esta biblioteca proporciona funciones para trabajar con la fecha y hora del sistema, utilizada para generar marcas de tiempo en los reportes generados por la aplicación.

### Mutex y Sincronización de Hilos

La aplicación utiliza objetos de mutex para garantizar la sincronización adecuada entre hilos y evitar condiciones de carrera al acceder y modificar datos compartidos. Se utilizan mutexes tanto para la carga masiva de usuarios como para la carga masiva de operaciones, asegurando la integridad de los datos durante el procesamiento concurrente.

Además, se utilizan variables globales para almacenar el estado de la carga de usuarios y operaciones, así como para contar el número total de usuarios y transacciones cargados. Estas variables globales son accesibles por todos los hilos y se protegen mediante mutexes para evitar inconsistencias de datos.

### Conclusión

El uso de mutexes y la sincronización adecuada de hilos garantizan que la aplicación bancaria pueda funcionar de manera segura y eficiente en un entorno concurrente. Al hacer uso de las bibliotecas estándar de C y técnicas de programación multiproceso, la aplicación puede manejar grandes volúmenes de datos de manera confiable, proporcionando una solución robusta para la gestión de usuarios y operaciones monetarias en un entorno bancario simulado.