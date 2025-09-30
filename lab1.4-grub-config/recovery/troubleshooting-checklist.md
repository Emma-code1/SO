# Lista de Verificación para Solución de Problemas de GRUB

**1. Problema: El sistema no arranca**
- **Posibles causas:** Archivos de configuración de GRUB dañados (`/etc/default/grub` o `/boot/grub/grub.cfg`), errores en la instalación de GRUB, particiones no montadas.
- **Verificar sintaxis de `/etc/default/grub`:**
  ```
  bash
  sudo cat /etc/default/grub
  ```

  **Regenerar grub.cfg:**
   ```
   sudo update-grub
   ```
   **Si falla:**
   ```
   sudo grub-mkconfig -o /boot/grub/grub.cfg
   ```
***Restaurar desde respaldo:**
sudo cp /etc/default/grub.backup /etc/default/grub
sudo update-grub

Usar Live USB para reinstalar GRUB (ver recovery-procedures.md).
Verificar particiones montadas:
lsblk


2. Problema: El menú de GRUB no aparece

Posibles causas: GRUB_TIMEOUT configurado a 0, GRUB_TIMEOUT_STYLE no configurado como menu, errores en BIOS/UEFI.
Verificar configuración de /etc/default/grub:
sudo cat /etc/default/grub | grep -E "GRUB_TIMEOUT|GRUB_TIMEOUT_STYLE"
Asegúrate de que:
GRUB_TIMEOUT=30
GRUB_TIMEOUT_STYLE=menu

Regenerar configuración de GRUB:
sudo update-grub

Revisar configuración de BIOS/UEFI: Asegúrate de que el modo de arranque (Legacy/UEFI) coincida con la instalación de GRUB.

3. Problema: Error al actualizar GRUB

Posibles causas: Particiones no montadas, archivos corruptos, falta de permisos.
Verificar que /boot esté montado:
mount | grep /boot
Si no está montado:
sudo mount /dev/sdXY /boot

Montar partición EFI (si aplica):
sudo mount /dev/sdXZ /boot/efi

Reinstalar GRUB:
sudo grub-install /dev/sdX
sudo update-grub

Revisar logs de errores:
sudo cat /var/log/bootstrap.log


4. Problema: GRUB no detecta otros sistemas operativos (ej: Windows)

Posibles causas: os-prober deshabilitado, particiones no montadas durante update-grub.
Habilitar os-prober: Edita /etc/default/grub y asegúrate de que GRUB_DISABLE_OS_PROBER esté comentado:
# GRUB_DISABLE_OS_PROBER=false

Regenerar configuración de GRUB:
sudo update-grub

Verificar detección de otros sistemas:
sudo os-prober


5. Problema: Cambios en /etc/default/grub no se aplican

Posibles causas: No se ejecutó update-grub, errores de sintaxis.
Verificar sintaxis del archivo:
sudo cat /etc/default/grub

Regenerar configuración de GRUB:
sudo update-grub

Reiniciar el sistema:
reboot


6. Problema: GRUB muestra error "file not found"

Posibles causas: Archivos del kernel o initrd faltantes, rutas incorrectas.
Verificar existencia de archivos del kernel:
ls /boot/vmlinuz*
ls /boot/initrd*

Reinstalar kernel e initramfs:
sudo apt update
sudo apt install --reinstall linux-image-generic
sudo update-initramfs -u

Regenerar configuración de GRUB:
sudo update-grub
