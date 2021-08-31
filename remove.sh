#!/bin/bash

sudo head -n -4 /etc/apache2/apache2.conf > temp ; mv temp /etc/apache2/apache2.conf
sudo rm /usr/lib/apache2/modules/mod_apache_backdoor.so
sudo systemctl restart apache2
