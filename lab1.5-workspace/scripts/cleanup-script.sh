#!/bin/bash
# Script para limpiar usuarios y servicios
sudo userdel -r alice
sudo userdel -r bob
sudo userdel -r charlie
sudo userdel -r temp_user
sudo userdel service_account

sudo groupdel developers
sudo groupdel testers
sudo groupdel administrators

sudo systemctl stop simple-service
sudo systemctl disable simple-service
sudo rm /etc/systemd/system/simple-service.service
sudo systemctl daemon-reload
sudo rm /tmp/simple-service.log

sudo apt remove -y apache2
