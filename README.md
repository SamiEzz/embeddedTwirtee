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


# 3. Localisation
## 3.1 Utilisation
La fonction "loc_thread()" est résponsable de renvoyer la position du robot en temps réel. il suffit de lancer un thread avec un objet de type (T_loc* ) comme ceci :
* T_loc* position
* pthread_create(&t_localisation, NULL, loc_thread, (void *)position)

la valeurs de position sera automatiquement et periodiquement stocké sur l'adresse de la variable position.


# 4. Service COM
