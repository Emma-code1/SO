# **Lab 1.4: GRUB Configuration and Boot Process Analysis**

**Student:** Emmanuel Castillo Leyva
**Date:** 29/09/2025
**Course:** Operating Systems

## **Executive Summary**
En este laboratorio, se analizó y modificó la configuración del gestor de arranque **GRUB** en una máquina virtual con Ubuntu. 
Se documentó el estado inicial del sistema, se realizaron cambios en el tiempo de espera del menú de arranque, y se investigaron métodos de recuperación en caso de fallos. 
El sistema se reinició exitosamente después de las modificaciones, y se validó que los cambios se aplicaron correctamente.

## **Objectives Completed**
- Creación de respaldos de seguridad de los archivos de configuración de GRUB.
- Análisis del estado actual de la configuración de GRUB.
- Modificación del tiempo de espera del menú de arranque a **30 segundos**.
- Modificación de parámetros adicionales en `/etc/default/grub`.
- Aplicación de cambios y verificación del funcionamiento del sistema.
- Investigación y documentación de métodos de recuperación de GRUB.
- Creación de scripts para automatizar respaldos y restauraciones.

## **Main Changes Implemented**
1. **Modificación del tiempo de espera del menú de arranque**:
   - `GRUB_TIMEOUT=30` (antes: `5` segundos).
2. **Personalización de parámetros**:
   - `GRUB_DEFAULT=0` (arranque predeterminado: primera entrada).
   - `GRUB_TIMEOUT_STYLE=menu` (muestra siempre el menú).
   - `GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"` (modo silencioso).
3. **Generación de nueva configuración**:
   - Ejecución de `sudo update-grub` para aplicar los cambios.

## **Key Learnings**
- **Configuración de GRUB**: Se aprendió a modificar parámetros críticos en `/etc/default/grub` y a regenerar la configuración con `update-grub`.
- **Recuperación del sistema**: Se investigó cómo acceder al modo de rescate de GRUB, montar particiones desde un Live USB, y usar `chroot` para reparar el sistema.
- **Automatización**: Se crearon scripts para respaldar y restaurar archivos de configuración, lo que facilita la recuperación en caso de errores.
- **Importancia de los respaldos**: La creación de snapshots y copias de seguridad fue clave para evitar la pérdida de datos durante las pruebas.

## **Evidence Files**
### **Documentación**

 | [`docs/pre-lab-analysis.md`](docs/pre-lab-analysis.md) | Análisis del estado inicial de GRUB. |
 | [`docs/configuration-changes.md`](docs/configuration-changes.md) | Cambios realizados |
 | [`docs/grub-recovery-guide.md`](docs/grub-recovery-guide.md) | Guía de recuperación de GRUB. |

### **Capturas de Pantalla**
 | [`screenshots/01-original-boot-menu.png`](screenshots/01-original-boot-menu.png) | Menú de arranque original. |
 | [`screenshots/02-grub-config-before.png`](screenshots/02-grub-config-before.png) | Configuración de GRUB antes de los cambios. |
 | [`screenshots/03-grub-config-after.png`](screenshots/03-grub-config-after.png) | Configuración de GRUB después de los cambios. |
 | [`screenshots/04-modified-boot-menu.png`](screenshots/04-modified-boot-menu.png) | Menú de arranque modificado. |
 | [`screenshots/05-update-grub-output.png`](screenshots/05-update-grub-output.png) | Salida del comando `update-grub`. |

### **Configuraciones**
 | [`configs/grub-default-original.txt`](configs/grub-default-original.txt) | Copia del archivo `/etc/default/grub` original. |
 | [`configs/grub-default-modified.txt`](configs/grub-default-modified.txt) | Copia del archivo `/etc/default/grub` modificado. |

### **Scripts**
 | [`scripts/grub-backup.sh`](scripts/grub-backup.sh) | Script para respaldar archivos de GRUB. |
 | [`scripts/grub-restore.sh`](scripts/grub-restore.sh) | Script para restaurar archivos de GRUB. |
 | [`scripts/system-info.sh`](scripts/system-info.sh) | Script para recolectar información del sistema. |

### **Recuperación**
 | [`recovery/recovery-procedures.md`](recovery/recovery-procedures.md) | Procedimientos de recuperación. |
 | [`recovery/grub-rescue-commands.md`](recovery/grub-rescue-commands.md) | Comandos útiles en modo rescate. |
 | [`recovery/troubleshooting-checklist.md`](recovery/troubleshooting-checklist.md) | Lista de verificación para solucionar problemas. |


