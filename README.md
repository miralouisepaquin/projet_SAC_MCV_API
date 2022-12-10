# projet_SAC_MCV_API

Projet terminal objets connectés 2022. 

Dans ce projet, nous voulons répondre au besoin d’un client qui aimerait que nous lui programmions un Système Atmosphérique Connectés (SAC). Ce client veut éventuellement en faire son outil de séchage de bois principal. Il possède déjà un système de four conventionnel qui n’est ni prédéfinie, ni égale, ce qui change le temps de collage et la qualité.

Voici ce qu’il aimerait avoir comme fonctionnalités :

    •Être capable de gérer la température du four. Un capteur de température serait suffisant pour gérer la température de celui-ci.
    •Un écran qui permettrait d’afficher des informations utiles, tel que les valeurs prédéfinies des temps de séchage selon le type de bois.
    •Ce même écran doit permettre de démarrer le dit four avec un affichage facile à comprendre.
    •Une façon de configurer les paramètres du four connecté à partir d’un réseau Wi-Fi ou Bluetooth.



## Auteur

- [@miralouisepaquin](https://www.github.com/miralouisepaquin)


## Installation

Pour installer la partie API, sur un linux utiliser les commandes suivantes et coller les dossiers approprié au bon endroits:

```bash
  npm install my-project
  cd my-project
```

Pour ce qui est de la partie locale, c'est un projet en C++ pour un ESP32.
Dans mon cas j'ai utiliser Visual Studio code avec plateformIO.
Créer un projet en utilsant la plateforme ESP32-DEV-KIT et coller les dossiers au bon endroits.

Il faut importer les bonnes librairies:

  	•adafruit/DHT sensor library@^1.4.4 
	•adafruit/Adafruit Unified Sensor@^1.1.6 
	•ESPAsyncWebServer-esphome 
	•AsyncTCP-esphome
	•bblanchon/ArduinoJson@^6.19.4
	•adafruit/Adafruit GFX Library@^1.11.3
	•adafruit/Adafruit SSD1306@^2.5.7
	•adafruit/Adafruit NeoPixel@^1.10.6
    
## Tech Stack

**Client:** C++, Json, JavaScript

**Server:** Node, Express


## Lessons Apprises

Le ESP32 permet un large éventail de possibilité. L'ajout de l'outil Wifi de ce petit appareil permet de configurer
un super produit en Démo de façon autonome.
