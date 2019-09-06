
# Emebedded Twirtee
# 1. Conversion cartographie
## 1.1 Outils à installer :
* dxfgrabber : pip install dxfgrabber
## 1.2 Utilisation
* Créer un fichier format dxf : "inputfile.dxf"
* Coller ce ficher dans : embeddedTwirtee/python_tools/map2json/
* Executer la commande suivante :
	* $ python3 main_extraction.py -i <inputfile.dxf> -o <outputfile.json>
* Ceci va créer un fichier json contenant les informations de la carte.


# 2. Gestion des mission
## 2.1 Utilisation
La gestion des missions se fait par la fonction "spf_thread()", : Short path first thread
cette fonction prends en paramètre un pointeur sur un objet du type  "spf_mission".
spf_mission contient le noeud de départ et d'arrivé ainsi que le chemin entre eux.
.
Cf. main.c

# 3. Localisation
## 3.1 Outils à installer
il faudrait télécharger l'API de Decawave prévu pour les carte DWM1001
disponible sur ce lien :  https://github.com/Decawave/dwm1001-examples

## 3.2 Utilisation
La fonction "loc_thread()" est résponsable de renvoyer la position du robot en temps réel. il suffit de lancer un thread avec un objet de type (T_loc* ) comme ceci :
* T_loc* position
* pthread_create(&t_localisation, NULL, loc_thread, (void *)position)
la valeur de la position sera automatiquement et périodiquement stocké sur l'adresse de la variable "position".
.
Cf. main.c
# 4. Service COM
## 4.1 Configuration
### 4.1.1 installation CAN
* git clone https://github.com/linux-can/can-utils.git
* cd can-utils
* ./autogen.sh
* ./configure
* make
* make install (sudo)
#### Enable the SPI interface by running raspi-config.
* cd /usr/bin
* sudo ./raspi-config
* séléctionnez A6 SPI et activer
#### ajouter les overlays
 sudo nano /boot/config.txt
#### Add these 3 lines to the end of file:
 * tparam=spi=on
* dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
* dtoverlay=spi-bcm2835-overlay
---  sudo reboot
#### Allumer l'interface CAN
sudo /sbin/ip link set can0 up type can bitrate 500000
https://www.youtube.com/watch?v=ZmGDCIQdqTw

## 4.2 Utilisation
Cf. l'exemple donnée dans le fichier main_can.c .
Et dans le fichier "./include/io_com_service/io_com_service.c"

# 5. Simulation
## 5.1 Utilisation

# Reférences :
* https://pypi.org/project/dxfgrabber/
* https://www.decawave.com/wp-content/uploads/2019/01/DWM1001-API-Guide-2.2.pdf
