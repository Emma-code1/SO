#!/bin/bash

# Script para recolectar información del sistema
# Uso: ./system-info.sh

echo "=== Información del Sistema ==="
echo "Nombre del host: $(hostname)"
echo "Usuario actual: $(whoami)"
echo "Fecha y hora: $(date)"
echo ""

echo "=== Versión del Sistema ==="
lsb_release -a
echo ""

echo "=== Versión del Kernel ==="
uname -a
echo ""

echo "=== Versión de GRUB ==="
grub-install --version
echo ""

echo "=== Particiones del Disco ==="
lsblk
echo ""

echo "=== Configuración Actual de GRUB ==="
cat /etc/default/grub
echo ""

echo "=== Información de CPU ==="
lscpu
echo ""

echo "=== Información de Memoria ==="
free -h
echo ""
