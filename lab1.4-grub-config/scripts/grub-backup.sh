#!/bin/bash

# Script para respaldar archivos de configuración de GRUB
# Uso: sudo ./grub-backup.sh

# Directorio de respaldos
BACKUP_DIR="/boot/grub/backups"
DATE=$(date +%Y%m%d_%H%M%S)

# Crear directorio de respaldos si no existe
echo "Creando directorio de respaldos en $BACKUP_DIR..."
sudo mkdir -p "$BACKUP_DIR"

# Respaldar archivos de configuración de GRUB
echo "Respaldando /etc/default/grub..."
sudo cp /etc/default/grub "$BACKUP_DIR/grub-default-$DATE.bak"

echo "Respaldando /boot/grub/grub.cfg..."
sudo cp /boot/grub/grub.cfg "$BACKUP_DIR/grub-cfg-$DATE.bak"

# Mostrar respaldos creados
echo "Respaldos creados en $BACKUP_DIR:"
ls -la "$BACKUP_DIR"
