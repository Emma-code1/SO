# Reporte de Configuración de Servicios

## Servicios Analizados
- **Apache2**: Servidor web instalado y configurado.
- **SSH**: Servicio de acceso remoto.
- **simple-service**: Servicio personalizado creado para el laboratorio.

## Detalles del Servicio Personalizado
- **Ubicación del script**: `/home/$USER/simple-service.sh`
- **Archivo de servicio**: `/etc/systemd/system/simple-service.service`
- **Función**: Escribe la fecha y hora en `/tmp/simple-service.log` cada 10 segundos.

## Dependencias del Servicio SSH
- Se analizaron las dependencias del servicio SSH usando `systemctl list-dependencies ssh`.

## Procedimientos de Solución de Problemas
- Se simuló un error en Apache2 moviendo su archivo de configuración.
- Se revisaron los logs para identificar el problema.
- Se restauró la configuración y se reinició el servicio.
