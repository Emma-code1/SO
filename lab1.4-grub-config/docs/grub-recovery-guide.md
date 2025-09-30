# Guía de Recuperación de GRUB


## **1. Introducción**
Esta guía proporciona los procedimientos necesarios para recuperar el gestor de arranque **GRUB** en sistemas Linux cuando:
- El sistema no arranca.
- El menú de GRUB no aparece.
- Se producen errores al actualizar GRUB.


## **2. Métodos de Recuperación**

### **2.1. GRUB Rescue Mode**
*¿Qué es?*
El modo rescate de GRUB es un entorno mínimo que se activa cuando GRUB no puede cargar su configuración normal.

*Cómo acceder:*
1. Reinicia el sistema.
2. Si GRUB falla, aparecerá el prompt:
   ```
   grub rescue>
   ```

*Comandos básicos:*
| Comando                     | Descripción                                                                                     |
|-----------------------------|-------------------------------------------------------------------------------------------------|
| ***`ls`***                  | Lista las particiones disponibles (ej: ***`(hd0)`***, ***`(hd0,msdos1)`***).                  |
| ***`set root=(hdX,Y)`***     | Establece la partición raíz donde está ***`/boot`*** (ej: ***`set root=(hd0,msdos1)`***).     |
| ***`insmod normal`***        | Carga el módulo ***`normal`*** para intentar recuperar el menú de GRUB.                       |
| ***`normal`***               | Intenta cargar el modo normal de GRUB.                                                          |
| ***`linux /boot/vmlinuz...`***| Carga manualmente el kernel de Linux.                                                          |
| ***`initrd /boot/initrd.img`***| Carga la imagen initramfs.                                                                     |
| ***`boot`***                 | Arranca el sistema con los parámetros configurados.                                            |
| ***`set prefix=(hdX,Y)/boot/grub`*** | Define la ruta de los archivos de GRUB.                                      |

*Procedimiento para arrancar manualmente:*
```
grub rescue> ls
grub rescue> set root=(hd0,msdos1)
grub rescue> insmod normal
grub rescue> normal
```
Si ***`normal`*** falla:
```
grub rescue> linux /boot/vmlinuz-5.15.0-76-generic root=/dev/sda1
grub rescue> initrd /boot/initrd.img-5.15.0-76-generic
grub rescue> boot
```

### **2.2. Recuperación desde Live USB/DVD**
*¿Qué es?*
Un Live USB/DVD es un medio de arranque con un sistema operativo temporal (ej: Ubuntu).

*Pasos para recuperar GRUB:*
1. Crea un Live USB con Ubuntu.
2. Arranca desde el Live USB.
3. Abre una terminal y ejecuta:
   ```
   sudo fdisk -l
   ```
4. Monta la partición raíz:
   ```
   sudo mount /dev/sdXY /mnt
   ```
   *(Reemplaza ***`sdXY`*** con tu partición raíz, ej: ***`sda2`***).*
5. Monta ***`/boot/efi`*** (si usas UEFI):
   ```
   sudo mount /dev/sdXZ /mnt/boot/efi
   ```
   *(Reemplaza ***`sdXZ`*** con la partición EFI, ej: ***`sda1`***).*
6. Usa ***`chroot`***:
   ```
   sudo chroot /mnt
   ```
7. Reinstala GRUB:
   ```
   grub-install /dev/sdX
   update-grub
   ```
   *(Reemplaza ***`sdX`*** con el disco, ej: ***`sda`***).*
8. Sal del ***`chroot`*** y reinicia:
   ```
   exit
   sudo umount /mnt
   reboot
   ```


### **2.3. Restauración de Respaldos**
*¿Qué es?*
Recuperar los archivos de configuración de GRUB desde copias de seguridad.

*Procedimiento:*
1. Restaurar ***`/etc/default/grub`***:
   ```
   sudo cp /etc/default/grub.backup /etc/default/grub
   ```
2. Regenerar ***`grub.cfg`***:
   ```
   sudo update-grub
   ```
   Si falla:
   ```
   sudo grub-mkconfig -o /boot/grub/grub.cfg
   ```
3. Verificar los cambios:
   ```
   sudo cat /boot/grub/grub.cfg | grep "menuentry"
   ```

## **3. Lista de Verificación para Solución de Problemas**

### **Problema: El sistema no arranca**
- Verificar sintaxis de ***`/etc/default/grub`***:
  ```
  sudo cat /etc/default/grub
  ```
- Regenerar ***`grub.cfg`***:
  ```
  sudo update-grub
  ```

### **Problema: El menú de GRUB no aparece**
- Verificar configuración de ***`/etc/default/grub`***:
  ```
  sudo cat /etc/default/grub | grep -E "GRUB_TIMEOUT|GRUB_TIMEOUT_STYLE"
  ```

### **Problema: Error al actualizar GRUB**
- Verificar que ***`/boot`*** esté montado:
  ```
  mount | grep /boot
  ```
