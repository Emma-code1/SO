# Reporte de Análisis del Sistema

## Recursos del Sistema
- **CPU**: $(lscpu | grep "Model name" | cut -d ":" -f 2)
- **Memoria**: $(free -h | grep "Mem:" | awk '{print $2}')
- **Disco**: $(df -h / | awk 'NR==2 {print $2}')

## Patrones de Utilización
- Los recursos del sistema se monitorearon usando `htop` y `neofetch`.
- Se generó un reporte de rendimiento con `system-performance.sh`.

## Recomendaciones de Monitoreo
- Usar `journalctl` para revisar logs en tiempo real.
- Monitorear el uso de disco y memoria regularmente.
