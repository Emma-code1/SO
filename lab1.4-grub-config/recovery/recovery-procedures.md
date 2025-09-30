# Procedimientos de Recuperación desde Live USB/DVD

## ¿Qué es un Live USB/DVD?
Un Live USB/DVD es un medio de arranque con un sistema operativo temporal (ej: Ubuntu). Se usa para reparar sistemas que no arrancan, como reinstalar GRUB o restaurar archivos.

---

## Pasos para recuperar GRUB

1. **Crea un Live USB**:
   - Descarga la ISO de Ubuntu desde [ubuntu.com](https://ubuntu.com/download).
   - Grábala en un USB con herramientas como **Rufus** o **Startup Disk Creator**.

2. **Arranca desde el Live USB**:
   - Reinicia el sistema y selecciona el USB en la BIOS/UEFI.

3. **Abre una terminal** en el entorno Live.

4. **Identifica las particiones:**
   ```
   sudo fdisk -l
   ```
6. **Monta la partición raíz:**
   ```
   sudo mount /dev/sdXY /mnt
   ```
   Reemplaza sdXY con tu partición raíz (sda2)
7. **Monta /boot/efi (Si usas UEFI):**
   ```
   sudo mount /dev/sdXZ /mnt/boot/efi
   ```
   Reemplaza sdXZ con la partición EFI
8. **Accede al sistema con chroot:**
   ```
   sudo chroot /mnt
   ```
9. **Reinstala GRUB:**
   ```
   grub-install /dev/sdX
   update-grub
   ```
   Reemplaza sdX con el disco (ej: sda).
10. **Sal del chroot y reinicia:**
   ```
   exit
   sudo umount /mnt
   reboot
   ```
