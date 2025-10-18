# Reporte de Gestión de Usuarios

## Usuarios Creados
- **alice**: Usuario estándar, miembro de los grupos `developers` y `administrators`.
- **bob**: Usuario estándar, miembro del grupo `developers`.
- **charlie**: Usuario estándar, miembro del grupo `testers`.
- **temp_user**: Usuario temporal con fecha de expiración 2025-12-31.
- **service_account**: Cuenta de sistema sin directorio home.

## Grupos Creados
- **developers**: Grupo para desarrolladores.
- **testers**: Grupo para probadores.
- **administrators**: Grupo para administradores.

## Permisos Configurados
- `/shared/projects`: Permisos 770, grupo `developers`.
- `/shared/testing`: Permisos 775, grupo `testers`.

## Consideraciones de Seguridad
- Se asignaron contraseñas seguras a todos los usuarios.
- Se configuró sudo para permitir a los `developers` reiniciar servicios web sin contraseña.
