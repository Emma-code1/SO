#!/bin/bash
# Script para crear usuarios
sudo useradd -m -s /bin/bash alice
sudo useradd -m -s /bin/bash bob
sudo useradd -m -s /bin/bash charlie
sudo useradd -m -s /bin/bash -e 2025-12-31 temp_user
sudo useradd -r -s /usr/sbin/nologin service_account

# Establecer contraseñas (ejecutar manualmente después)
echo "Ejecutar manualmente: sudo passwd alice, sudo passwd bob, sudo passwd charlie, sudo passwd temp_user"
