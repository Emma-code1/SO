# Análisis Pre-Laboratorio: Configuración Actual de GRUB

## **Información del Sistema**
- **Distribución:** Ubuntu [versión]
- **Versión de GRUB:**
  ```
  sudo grub-install --version
  ```
  Salida: ***grub-install (GRUB) 2.04-1ubuntu26.14***

---

## **Configuración Actual de GRUB**

### **Archivo ***/etc/default/grub***
```
sudo cat /etc/default/grub
```
Contenido:
```ini
GRUB_DEFAULT=0
GRUB_TIMEOUT=5
GRUB_DISTRIBUTOR=`lsb_release -i -s 2> /dev/null || echo Debian`
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
GRUB_CMDLINE_LINUX=""
```

### **Valores Clave**
| Parámetro                     | Valor Actual       |
|-------------------------------|--------------------|
| ***GRUB_TIMEOUT***            | 5                  |
| ***GRUB_DEFAULT***            | 0                  |
| ***GRUB_CMDLINE_LINUX_DEFAULT*** | ***"quiet splash"*** |
| Número de entradas en el menú | 13                  |

---

## **Análisis del Menú de Arranque**
- **Tiempo de espera:** 5 segundos.
- **Entradas en el menú:**
  1. Ubuntu
  2. Advanced options for Ubuntu
  3. Memory test (memtest86+x64.bin)
  4. Memory test (memtest86+x64.bin, serial console)


- **Captura de pantalla:** ***[`../screenshots/01-original-boot-menu.png`](../screenshots/01-original-boot-menu.png)***
