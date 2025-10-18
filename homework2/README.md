cat > README.md << 'EOF'
# Zombie Process Monitor and Reaper
**Estudiante:** Emmanuel Castillo Leyva **Fecha:** 29/09/2025 Curso: Sistemas Operativos


Este proyecto implementa herramientas para crear, detectar, y eliminar procesos zombies en Linux.

## Contenido

- **zombie_creator**: Crea procesos zombies para pruebas.
- **zombie_detector**: Detecta procesos zombies en el sistema.
- **zombie_reaper**: Elimina procesos zombies usando diferentes estrategias.
- **process_daemon**: Un daemon que nunca crea procesos zombies.
- **libzombie.a**: Librería reutilizable para prevenir procesos zombies.

## Compilación

Ejecuta `make` para compilar todos los programas.

## Uso

1. **Crear procesos zombies**:
   ```bash
   ./zombie_creator 5
   ```
2. **Detectar procesos zombies**:
   ```bash
   ./zombie_detector
   ```
3. **Eliminar procesos zombies**:
   ```bash
   ./zombie_reaper 1 # Usa estrategia 1,2 o 3
   ```
4. **Ejecutar el daemon**:
   ```bash
   ./process_daemon
   ```
5. **Ejemplo de la librería**:
   ```bash
   ./example
   ```

## Pruebas

Los scripts de prueba están en la carpeta tests/
