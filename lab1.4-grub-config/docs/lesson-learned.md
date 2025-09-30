# Lecciones Aprendidas


## **Configuración de GRUB**
- **Importancia de los respaldos:** Siempre crear copias de seguridad antes de modificar archivos críticos como ***`/etc/default/grub`***.
- **Verificación:** Usar ***`update-grub`*** después de cada cambio y revisar la salida para detectar errores.
- **Documentación:** Registrar los valores originales y los cambios realizados para facilitar la recuperación.


## **Recuperación del Sistema**
- **Modo rescate:** Conocer los comandos básicos de GRUB (***`ls`***, ***`set`***, ***`insmod`***) es esencial para solucionar problemas de arranque.
- **Live USB:** Herramienta indispensable para reparar sistemas que no arrancan.
- **`chroot`:** Permite ejecutar comandos en el sistema instalado desde un entorno Live.


## **Automatización**
- Los scripts de respaldo y restauración (***`grub-backup.sh`***, ***`grub-restore.sh`***) ahorran tiempo y reducen errores humanos.

