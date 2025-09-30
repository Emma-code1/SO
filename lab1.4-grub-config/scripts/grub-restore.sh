#!/bin/bash

# Script para restaurar archivos de configuración de GRUB desde respaldos
# Uso: sudo ./grub-restore.sh

# Directorio de respaldos
BACKUP_DIR="/boot/grub/backups"

# Listar respaldos disponibles
echo "Respaldos disponibles en $BACKUP_DIR:"
ls -t "$BACKUP_DIR"

# Solicitar confirmación
read -p "¿Quieres restaurar el último respaldo? (s/n): " confirm

if [ "$confirm" = "s" ]; then
    # Obtener el último respaldo
    LATEST_GRUB_DEFAULT=$(ls -t "$BACKUP_DIR"/grub-default-*.bak | head -1)
    LATEST_GRUB_CFG=$(ls -t "$BACKUP_DIR"/grub-cfg-*.bak | head -1)

    echo "Restaurando $LATEST_GRUB_DEFAULT a /etc/default/grub..."
    sudo cp "$LATEST_GRUB_DEFAULT" /etc/default/grub

    echo "Restaurando $LATEST_GRUB_CFG a /boot/grub/grub.cfg..."
    sudo cp "$LATEST_GRUB_CFG" /boot/grub/grub.cfg

    echo "Archivos restaurados. Actualizando GRUB..."
    sudo update-grub

    echo "Proceso completado."
else
    echo "Operación cancelada."
fi
