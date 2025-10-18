# Reporte de Solución de Problemas

## Problema con Apache2
- **Error**: Apache2 no pudo iniciarse después de mover su archivo de configuración.
- **Solución**:
  1. Restaurar el archivo de configuración desde el respaldo.
  2. Reiniciar el servicio Apache2.

## Logs Relevantes
- Se revisaron los logs de Apache2 para identificar el error.
- Se usó `journalctl -u apache2` para ver los mensajes de error.

## Lecciones Aprendidas
- Siempre respaldar archivos de configuración antes de modificarlos.
- Usar `journalctl` para diagnosticar problemas con servicios.
