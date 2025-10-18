# Administrador de Procesos en C
**Estudiante**: Emmanuel Castillo Leyva
**Matrícula**: 5062029
**Correo**: ecastillo@edu.uag.mx
**Fecha**: 17/10/2025

## Descripción
Implementa un **administrador de procesos** en C que permita:
- Crear y gestionar procesos hijos.
- Ejecutar programas externos.
- Monitorear el estado de los procesos.
- Manejar señales para un cierre limpio.
- Visualizar el árbol de procesos.
- Evitar procesos zombis.

---

## Funcionalidades Clave

### 1. Creación y Gestión de Procesos
- **`create_process`**: Crea un proceso hijo y ejecuta un comando usando `fork()` y `execvp()`.
- **`check_process_status`**: Verifica si un proceso sigue en ejecución con `waitpid()`.
- **`terminate_process`**: Termina un proceso con `SIGTERM` o `SIGKILL`.

### 2. Tabla de Procesos
- Almacena información de los procesos (PID, comando, estado).
- **`list_processes`**: Muestra la lista de procesos en formato de tabla.
- **`wait_all_processes`**: Espera a que todos los procesos terminen.

### 3. Manejo de Señales
- **`SIGINT` (Ctrl+C)**: Cierra el programa de manera ordenada.
- **`SIGCHLD`**: Elimina procesos zombis y actualiza la tabla.

### 4. Visualización del Árbol de Procesos
- **`print_process_tree`**: Muestra la jerarquía de procesos en formato de árbol.

### 5. Interfaz Interactiva
Comandos disponibles:
- `create <comando> [args...]`: Crea un nuevo proceso.
- `list`: Lista todos los procesos.
- `kill <pid> [force]`: Termina un proceso.
- `tree`: Muestra el árbol de procesos.
- `wait`: Espera a que todos los procesos terminen.
- `quit`: Sale del programa.

---

## Estructura del Proyecto
homework1/
├── procman.c
├── procman.h
├── Makefile
├── README.md
└── test_scripts/
   ├── test1.sh
   ├── test2.sh
   └── test3.sh

**Nota:** Usa herramientas como `strace`, `gdb` y `valgrind` para depurar.
