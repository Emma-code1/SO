# Lab 1.5: Users and Services Management

**Estudiante:** Emmanuel Castillo Leyva
**Fecha:** 29/09/2025
**Curso:** Sistemas Operativos

## Resumen Ejecutivo
En este laboratorio, se realizaron tareas administrativas en un sistema Linux, incluyendo:
- Gestión de usuarios y grupos.
- Configuración de permisos y sudo.
- Administración y monitoreo de servicios.
- Creación de un servicio personalizado.
- Análisis de logs y solución de problemas.

## Tareas Completadas
- Creación y gestión de usuarios y grupos.
- Configuración de permisos en directorios compartidos.
- Instalación y configuración de servicios (Apache2, servicio personalizado).
- Monitoreo de recursos del sistema y análisis de logs.

## Usuarios y Grupos Creados
| Usuario         | Grupos               | Directorio Home | Shell          | Propósito               |
|-----------------|----------------------|-----------------|----------------|-------------------------|
| alice           | developers, administrators | /home/alice     | /bin/bash      | Desarrollador principal |
| bob             | developers           | /home/bob       | /bin/bash      | Desarrollador junior    |
| charlie         | testers              | /home/charlie   | /bin/bash      | Probador de QA          |
| temp_user       | -                    | /home/temp_user | /bin/bash      | Usuario temporal        |
| service_account | -                    | -               | /usr/sbin/nologin | Cuenta de sistema    |

## Servicios Configurados
- Apache2
- simple-service (servicio personalizado)
- SSH

## Aprendizajes Clave
- Cómo crear y gestionar usuarios y grupos en Linux.
- Configuración de permisos y uso de `sudo`.
- Administración de servicios con `systemctl`.
- Monitoreo de recursos y análisis de logs.

## Medidas de Seguridad Implementadas
- Asignación de contraseñas seguras.
- Configuración de permisos restrictivos en directorios compartidos.
- Uso de grupos para gestionar accesos y privilegios.
