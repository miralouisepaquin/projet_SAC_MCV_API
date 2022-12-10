/* Copyright (C) 2022 Mira Paquin
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2022
 *  
    @file     main.cpp
    @author   Mira Paquin
    @version  3.1 22/12/09 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        22/09/15    AD        Première version du logiciel
           2.1        22/11/25    MLP       Système fonctionnel grâce à une page web,connexion en Wifi, 
                                            gestion des fonction grâce au MyServer et logique applicative fonctionnel
                                            (si la température descend sous le seuil, soit +-5% de la température cible, le temps de 
                                            séchage = 0. Si la température dépasse le seuil, soit +-5% de la température cible, le temps de 
                                            séchage se met en pause)
           3.1        22/12/01    MLP       Création des vue du Oled afin d'afficher les différents
                                            états du processus

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps =                       
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les fonctionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)
            adafruit/DHT sensor library@^1.4.4          (Pour la lecture de température)
            adafruit/Adafruit Unified Sensor@^1.1.6     (Pour la lecture de température)
            adafruit/Adafruit GFX Library@^1.11.3       (Pour la utiliser le Oled)
            adafruit/Adafruit SSD1306@^2.5.7            (Pour la utiliser le Oled)
            adafruit/Adafruit NeoPixel@^1.10.6          (Pour la utiliser le Oled)

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
        Fonctions utiles (utilitaires)
            /lib/MYLIB/myFunctions.cpp
                //Pour vérifier plus simplement que deux chaines sont identiques
                bool isEqualString(std::string line1, std::string line2)
                //Pour extraire une partie d'une chaine de caractères avec l'aide d'un index
                std::string getValue(std::string data, char separator, int index)
                //Pour remplacer plus facilement une sous chaine
                bool replaceAll(std::string& source, const std::string& from, const std::string& to)
                //Pour obtenir un chaine aléatoire d'une certaine longeur
                std::string get_random_string(unsigned int len)

    Classes du système         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
        /data                                   Répertoire qui contient les fichiers du site WEB 
            index.html                  V1.0    Page index du site WEB
            index.css                   V1.0    CSS
            script.js                   V1.0    JS (fonctions JavaScript)
            SAC.png                     V1.0    image Logo Compagnie
        MyButton                        V1.0    Pour gérer un (ou des) bouton(s) Touch de l'ESP32 
            GPIO33 : T9                 V1.0    Pour le bouton RESET 
            GPIO32 : T8                 V1.0    Pour le bouton ACTION
        TemperatureStub                 V1.0    Senseur de température et d'humidité DS18B20 / DHT11
            GPIO15 : 
        MyOled                          V1.0    Pour utiliser le Oled
        MyOledView                      V1.0    Pour initialiser les vue du Oled
        MyOledViewErrorWifiConnexion    V1.0    Pour la vue du Oled lors d'erreur de connexion Wifi
        MyOledWifiAp                    V1.0    Pour la vue des identifiants du Wifi
        MyOledViewWorking               V1.0    Pour la vue initiale du Oled
        MyOledViewWorkingCold           V1.0    Pour la vue lorsque la température est hors seuil
        MyOledViewWorkingHeat           V1.0    Pour la vue lorsque le four chauffe
        MyOledViewWorkingOff            V1.0    Pour la vue lorsque le four est arrêter


    Configuration du système
        GPIO12 : LEDs Rouge 
        GPIO14 : LEDs Vert                            
        GPIO27 : LEDs Jaune              
* */

#include <iostream>
#include <string>
#include <Arduino.h>
#include "MyOled.h"
#include "myFunctions.cpp"

using namespace std;

#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Pour la gestion des boutons
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;

//Pour avoir les données du senseur de température
#include "TemperatureStub.h"
#define DHTPIN  15   
#define DHTTYPE DHT22 
TemperatureStub *temperatureStub = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom; 

//Définition des trois leds de statut
#define GPIO_PIN_LED_LOCK_ROUGE 12
#define GPIO_PIN_LED_OK_GREEN 14
#define GPIO_PIN_LED_HEAT_YELLOW 27

//Variable pour vitesse baud Serial
#define BAUD 9600

//Variable Oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define OLED_I2C_ADDRESS 0x3C

#include "MyOled.h"
MyOled *myOled = new MyOled(&Wire,OLED_RESET,SCREEN_HEIGHT,SCREEN_WIDTH);
#include "MyOledView.h"
MyOledView *myOledView = NULL;
#include "MyOledViewWorking.h"
MyOledViewWorking *myOledViewWorking = NULL;
#include "MyOledViewInitialisation.h"
MyOledViewInitialisation *myOledViewInitialisation = NULL;
#include "MyOledViewWifiAp.h"
MyOledViewWifiAp *myOledViewWifiAp = NULL;
#include "MyOledViewErrorWifiConnexion.h"
MyOledViewErrorWifiConnexion *myOledViewErrorWifiConnexion = NULL;
#include "MyOledViewWorkingOFF.h"
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;
#include "MyOledViewWorkingCold.h"
MyOledViewWorkingCold *myOledViewWorkingCold = NULL;
#include "MyOledViewWorkingHEAT.h"
MyOledViewWorkingHeat *myOledViewWorkingHeat = NULL;

#define NomSysteme "Four SAC"
#define IdSysteme "F8563"
//Variables pour temperature Actuelle
std::string temperatureActuelle = "22";
char buffertemperatureActuelle[50];
//Variables pour temps de Séchage actuel
int tempsSechageActuel = 0;
std::string temps = "0";
char bufferTempsSechageActuel[50];
//Variables pour temps de séchage maximum
int tempsSechageMax = 20;
String stringTempsSechageMax = "0";
//Variables pour température Minimum
float temperatureMin = 23.0;
String stringTemperatureMin = "0";

//Variables pour les options du bois choisit
String nomBois="";
String typeBois="";
String origineBois="";
//variable pour l'affichage Web de l'état de la lumière
std::string bkColor = "green";
//variable pour État Boutton Démarrer de la page Web
bool demarrer = 0;
//Variable pour modéré la vitesse d'éxécution de l'app
int modulo = 0;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    string arg2 = getValue(message, ' ', 2);
    string arg3 = getValue(message, ' ', 3);
    string arg4 = getValue(message, ' ', 4);
    string arg5 = getValue(message, ' ', 5);
    string arg6 = getValue(message, ' ', 6);
    string arg7 = getValue(message, ' ', 7);
    string arg8 = getValue(message, ' ', 8);
    string arg9 = getValue(message, ' ', 9);
    string arg10 = getValue(message, ' ', 10);
    
    if (string(actionToDo.c_str()).compare(string("askTemperature")) == 0) {     
        return(temperatureActuelle.c_str()); }  

    if (string(actionToDo.c_str()).compare(string("askTemps")) == 0) {     
        return(temps.c_str()); }

    if (string(actionToDo.c_str()).compare(string("askBkColor")) == 0) {     
        return(bkColor.c_str()); }

    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {                          
        demarrer = 1;          
        return("");  
        } 
    
    if (string(actionToDo.c_str()).compare(string("woodOption")) == 0) { 
        nomBois=arg1.c_str();
        origineBois=arg3.c_str();
        typeBois=arg2.c_str();
        stringTempsSechageMax = arg4.c_str();
        stringTemperatureMin = arg5.c_str();
        return("");
        }

   
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(BAUD);
    delay(100);
    //---------Initialisation des view Oled-----------
    myOled->init(OLED_I2C_ADDRESS);
    myOled->veilleDelay(30); //En secondes
  
    myOledViewInitialisation = new MyOledViewInitialisation();
    myOledViewInitialisation->setNomDuSysteme(NomSysteme);
    myOledViewInitialisation->setIdDuSysteme(IdSysteme);
    myOledViewInitialisation->setSensibiliteBoutonAction("????");
    myOledViewInitialisation->setSensibiliteBoutonReset("????");
    myOled->displayView(myOledViewInitialisation);
    myOledViewWifiAp = new MyOledViewWifiAp();
    myOledViewErrorWifiConnexion = new MyOledViewErrorWifiConnexion();  
    myOledViewWorkingOFF = new MyOledViewWorkingOFF();
    myOledViewWorkingOFF->setParams("nomDuSysteme",NomSysteme);
    myOledViewWorkingOFF->setParams("idDuSysteme",IdSysteme);
    myOledViewWorkingCold = new MyOledViewWorkingCold();
    myOledViewWorkingCold->setParams("nomDuSysteme",NomSysteme);
    myOledViewWorkingCold->setParams("idDuSysteme",IdSysteme);
    myOledViewWorkingHeat = new MyOledViewWorkingHeat();
    myOledViewWorkingHeat->setParams("nomDuSysteme",NomSysteme);
    myOledViewWorkingHeat->setParams("idDuSysteme",IdSysteme);
    //---------------Gestion des boutons------------------
    myButtonAction = new MyButton();
    myButtonAction->init(T8);
    int sensibilisationButtonAction = myButtonAction->autoSensibilisation();

    myButtonReset = new MyButton();
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();

    myOledViewInitialisation->setSensibiliteBoutonAction(String(sensibilisationButtonAction).c_str());
    myOledViewInitialisation->setSensibiliteBoutonReset(String(sensibilisationButtonReset).c_str());
    myOled->displayView(myOledViewInitialisation);
    delay(1000);

    //------Initialisation des LED statuts----------
    pinMode(GPIO_PIN_LED_LOCK_ROUGE,OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN,OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_YELLOW,OUTPUT);

    //---------Connection au WifiManager--------------
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

    char strToPrint[128];
    myOledViewWifiAp->setNomDuSysteme(NomSysteme);
    myOledViewWifiAp->setSsIdDuSysteme(ssIDRandom.c_str());
    myOledViewWifiAp->setPassDuSysteme(PASSRandom.c_str());
    myOled->displayView(myOledViewWifiAp);
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
    Serial.println(strToPrint);  

    if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        myOledViewErrorWifiConnexion->setNomDuSysteme(NomSysteme);
        myOled->displayView(myOledViewErrorWifiConnexion);
        }
    else {
        for (int i = 0; i < 2; i++)
        {
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
            digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
            digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
            delay(500);
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
            digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
            digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW);
            delay(500);
            }  
            myOledViewWorkingOFF->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());   
            myOledViewWorkingCold->setParams("ipDuSysteme",WiFi.localIP().toString().c_str()); 
            myOledViewWorkingHeat->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());                  
        }

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

    //-----Initiation pour la lecture de la température---------
    temperatureStub = new TemperatureStub;
    if(!temperatureStub)while(1);
    
    temperatureStub->init(DHTPIN, DHTTYPE);
    }

void loop() {  
    //Gestion du bouton Action
    int buttonAction = myButtonAction->checkMyButton();
    if(buttonAction > 2)  {  //Si appuyé plus de 0.2 secondes
        Serial.println("Button Action pressed : ");
        Serial.println(buttonAction);
        }

    //Gestion du bouton Reset
    int buttonReset = myButtonReset->checkMyButton();
    if(buttonReset > 300)  {  //Si appuyé plus de 30 secondes
        Serial.println("Button Reset pressed\n");
        Serial.println("Button Hard reset pressed\n");
        Serial.println("Suppression des réglages et redémarrage...\n");
        wm.resetSettings();
        ESP.restart();
        }
    //---------------Section gestion Température---------------
    if(modulo%100 == 0){
        float t = temperatureStub->getTemperature();
    
        sprintf(buffertemperatureActuelle,"%2.1f",t);
        temperatureActuelle = String(buffertemperatureActuelle).c_str();

        if (demarrer == 0)
        {
            if(digitalRead(GPIO_PIN_LED_OK_GREEN) == false)digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
            bkColor="green";
            tempsSechageMax = std::stof(stringTempsSechageMax.c_str());
            temperatureMin = std::stof(stringTemperatureMin.c_str());
            myOled->displayView(myOledViewWorkingOFF);                    
            }    
        
        if(demarrer == 1){              
            if(tempsSechageActuel <= tempsSechageMax){                          
                if((t >= (temperatureMin-(temperatureMin*0.05)))&&(t <= (temperatureMin+(temperatureMin*0.05)))){
                    if(digitalRead(GPIO_PIN_LED_OK_GREEN) == true)digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
                    if(digitalRead(GPIO_PIN_LED_HEAT_YELLOW) == true)digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW); 
                    if(digitalRead(GPIO_PIN_LED_LOCK_ROUGE) == false)digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
                    bkColor="red";
                    myOledViewWorkingHeat->setParams("temperature",temperatureActuelle);
                    myOled->displayView(myOledViewWorkingHeat);
                    tempsSechageActuel++;
                    sprintf(bufferTempsSechageActuel,"%i",tempsSechageActuel);
                    temps = String(bufferTempsSechageActuel).c_str();                                  
                    }
                else{
                    if(digitalRead(GPIO_PIN_LED_OK_GREEN) == true)digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
                    if(digitalRead(GPIO_PIN_LED_LOCK_ROUGE) == true)digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
                    if(digitalRead(GPIO_PIN_LED_HEAT_YELLOW) == false)digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
                    bkColor="yellow";
                    myOledViewWorkingCold->setParams("temperature",temperatureActuelle);
                    myOled->displayView(myOledViewWorkingCold);
                    if(t < (temperatureMin-(temperatureMin*0.05)))tempsSechageActuel=0;               
                    }
                }  
            if(tempsSechageActuel >= tempsSechageMax){
                if(digitalRead(GPIO_PIN_LED_LOCK_ROUGE) == true)digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
                if(digitalRead(GPIO_PIN_LED_HEAT_YELLOW) == true)digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW);
                if(digitalRead(GPIO_PIN_LED_OK_GREEN) == false)digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
                bkColor="green";
                myOled->displayView(myOledViewWorkingOFF);
                demarrer=0;
                tempsSechageActuel=0;
                }              
            }
        }
    modulo++;  
    delay(10);      
    }
