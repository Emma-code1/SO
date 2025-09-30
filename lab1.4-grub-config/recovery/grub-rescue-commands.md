# Comandos para GRUB Rescue Mode

## ¿Qué es GRUB Rescue Mode?
El modo rescate de GRUB es un entorno mínimo que se activa cuando GRUB no puede cargar su configuración normal. Permite reparar el arranque del sistema mediante comandos manuales.

## Cómo acceder
1. Reinicia el sistema.
2. Si GRUB falla, aparecerá automáticamente el prompt:
   ```
   grub rescue>
   ```

## Comandos básicos

| Comando                        | Descripción                                                                                     |
|--------------------------------|-------------------------------------------------------------------------------------------------|
| `ls`                           | Lista las particiones disponibles (ej: `(hd0)`, `(hd0,msdos1)`).                              |
| `set root=(hdX,Y)`             | Establece la partición raíz donde está `/boot` (ej: `set root=(hd0,msdos1)`).                  |
| `insmod normal`                | Carga el módulo `normal` para intentar recuperar el menú de GRUB.                              |
| `normal`                       | Intenta cargar el modo normal de GRUB.                                                          |
| `linux /boot/vmlinuz...`       | Carga manualmente el kernel de Linux (ej: `linux /boot/vmlinuz-5.15.0-76-generic root=/dev/sda1`).|
| `initrd /boot/initrd.img`      | Carga la imagen initramfs (ej: `initrd /boot/initrd.img-5.15.0-76-generic`).                    |
| `boot`                         | Arranca el sistema con los parámetros configurados.                                            |
| `set prefix=(hdX,Y)/boot/grub` | Define la ruta de los archivos de GRUB.                                                      |


## Procedimiento para arrancar manualmente
1. Identifica la partición de `/boot`:
   ```
   grub rescue> ls
   ```
2. Establece la partición raíz:
   ```
   grub rescue< set root=(hd0,msdos1)
   ```
4. Carga los módulos necesarios:
   ```
   grub rescue> insmod normal
   grub rescue> normal
   ```
6. Si normal falla, carga el kernel manualmente:
   ```
   grub rescue> linux /boot/vmlinuz-5.15.0-76-generic root=/dev/sda1
   grub rescue> initrd /boot/initrd.img-5.15.0-76-generic
   grub rescue> boot
   ```
