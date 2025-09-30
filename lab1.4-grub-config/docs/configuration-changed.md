# Cambios Realizados en la Configuración de GRUB

## **Modificaciones en ***/etc/default/grub***
| Parámetro                     | Valor Anterior | Valor Nuevo       | Justificación                                  |
|-------------------------------|----------------|-------------------|------------------------------------------------|
| ***GRUB_TIMEOUT***            | 5              | 30                | Aumentar el tiempo para seleccionar opciones.  |
| ***GRUB_DEFAULT***            | 0              | 0                 | Mantener el arranque predeterminado.           |
| ***GRUB_TIMEOUT_STYLE***      | (no definido)  | ***menu***        | Mostrar siempre el menú de arranque.           |
| ***GRUB_CMDLINE_LINUX_DEFAULT*** | ***"quiet splash"*** | ***"quiet splash"*** | Sin cambios.                                   |
| ***GRUB_CMDLINE_LINUX***      | ""             | ""                | Sin cambios.                                   |

## **Procedimiento**
1. **Editar el archivo:**
   ```
   sudo nano /etc/default/grub
   ```

2. **Aplicar cambios:**
   ```
   sudo update-grub
   ```

3. **Verificar salida:** ***[`../screenshots/05-update-grub-output.png`](../screenshots/05-update-grub-output.png)***

---

## **Capturas de Pantalla**
- **Antes:** ***[`../screenshots/02-grub-config-before.png`](../screenshots/02-grub-config-before.png)***
- **Después:** ***[`../screenshots/03-grub-config-after.png`](../screenshots/03-grub-config-after.png)***
- **Menú modificado:** ***[`../screenshots/04-modified-boot-menu.png`](../screenshots/04-modified-boot-menu.png)***
