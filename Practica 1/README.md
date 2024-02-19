**Universidad De San Carlos De Guatemala**
**Facultad De Ingeniería**
**Escuela De Ciencias Y Sistemas**
**Sistemas Operativos 2 Sección A**
**Sebastian Alejandro de Leon Tenaz**
**201906085**

**Práctica 1 (Llamadas a sistema)**

**Manual Técnico**

---

**1. Introducción**

En esta práctica, se desarrolló un programa en C para entender y aplicar varios conceptos fundamentales de sistemas operativos. Se abordaron temas como la creación de procesos hijos mediante la llamada al sistema `fork()`, ejecución de programas externos utilizando `execv()`, monitoreo de llamadas al sistema utilizando `SystemTap`, y manejo de señales del sistema, específicamente la señal SIGINT, utilizando `signal.h`.

**2. Conceptos clave**

- **fork()**: Es una llamada al sistema utilizada para crear un nuevo proceso duplicando el proceso que la llama. Después de una llamada a `fork()`, ambos procesos, el padre y el hijo, tienen su propio espacio de memoria y recursos del sistema.

- **execv()**: Esta función del sistema se utiliza para ejecutar un programa en un proceso existente. Reemplaza la imagen del proceso actual con un nuevo programa especificado.

- **SystemTap**: Es una herramienta diseñada para recolectar información del kernel con el propósito de monitorear el funcionamiento del sistema operativo. Se puede usar para registrar y analizar eventos del sistema en tiempo real.

- **Señal SIGINT**: Es una señal de interrupción enviada a un proceso en el terminal por el usuario (generalmente presionando Ctrl+C). Es utilizada para solicitar la interrupción de un proceso.

**3. Descripción del programa**

El programa creado consiste en la creación de dos procesos hijos utilizando `fork()`. En el proceso hijo, se utiliza `execv()` para ejecutar un programa externo llamado `child1.bin`, que realiza operaciones aleatorias de read, write o seek en un archivo llamado `practica1.txt`.

Además, se utiliza `SystemTap` desde el proceso padre para monitorear las llamadas al sistema de lectura y escritura en el archivo `syscalls.log`.

El programa también detecta la señal SIGINT (Ctrl+C) utilizando `signal.h`, y antes de finalizar, muestra la cantidad de llamadas al sistema realizadas por ambos hijos, así como el número de llamadas al sistema de lectura y escritura.

**4. Estructura del programa**

El programa consta de las siguientes secciones principales:

- **Creación de procesos hijos**: Se utiliza `fork()` para crear dos procesos hijos. En el proceso hijo, se utiliza `execv()` para ejecutar el programa externo `child1.bin`.

- **Monitoreo de llamadas al sistema**: Desde el proceso padre, se utiliza `SystemTap` para monitorear las llamadas al sistema de lectura y escritura, registrándolas en el archivo `syscalls.log`.

- **Manejo de señales**: Se utiliza `signal.h` para detectar la señal SIGINT. Cuando se detecta esta señal, el programa muestra estadísticas sobre las llamadas al sistema realizadas por los hijos.

**5. Ejecución del programa**

El programa puede ser compilado y ejecutado en un entorno Unix/Linux utilizando un compilador de C compatible. Para ejecutar el programa, simplemente se debe ejecutar el binario resultante.

**6. Conclusiones**

Esta práctica proporciona una comprensión práctica de conceptos fundamentales de sistemas operativos, como la creación de procesos, ejecución de programas externos, monitoreo de llamadas al sistema y manejo de señales del sistema. Estos conceptos son cruciales para el desarrollo de aplicaciones y sistemas robustos y eficientes en entornos operativos.

