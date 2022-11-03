/* Copyright (C) 2021 Alain Dube
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 21/08/15 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain       Première version du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
                      
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les focntionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)

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
            /data                               Répertoire qui contient les fichiers du site WEB 
                index.html              V1.0    Page index du site WEB
                index.css               V1.0    CSS
                script.js               V1.0    JS (fonctions JavaScript)
                SAC.png                 V1.0    image Logo Compagnie
              
 * */

#include <iostream>
#include <string>

#include <Arduino.h>

#include "myFunctions.cpp" //fonctions utilitaires

using namespace std;

#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Pour avoir les données du senseur de température
#include "TemperatureStub.h"
#define DHTPIN  15   // Pin utilisée par le senseur DHT11 / DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé (mais ce serait mieux d'avoir des DHT22 pour plus de précision)
TemperatureStub *temperatureStub = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom; 

//Définition des trois leds de statut
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12
#define GPIO_PIN_LED_OK_GREEN             14 //GPIO14
#define GPIO_PIN_LED_HEAT_YELLOW        27 //GPIO27

//Temperature
std::string temperature = "22";
char bufferTemperature[1024];
//Temps de Séchage
int tempsSechage = 0;
std::string temps = "0";
char bufferTemps[1024];
int tempsSechageMax = 20;
//Température Minimum
float temperatureMin = 23.0;
std::string stringTemperatureMin = "0";
char bufferTemperatureMin[1024];
//BackGroundColors
std::string bkColor = "red";
//Boutton Démarrer
bool demarrer = 0;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    //Nom du Four
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
        return(temperature.c_str()); } 

    if (string(actionToDo.c_str()).compare(string("askTemperatureMin")) == 0) {    
        return(stringTemperatureMin.c_str()); } 

    if (string(actionToDo.c_str()).compare(string("askTemps")) == 0) {     
        return(temps.c_str()); }

    if (string(actionToDo.c_str()).compare(string("askBkColor")) == 0) {     
        return(bkColor.c_str()); }

    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {                          
        demarrer = 1;          
        return("");  
        } 

   
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);

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
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
    Serial.println(strToPrint);


    if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        Serial.println("Erreur de connexion.");
      
        }
    else {
        Serial.println("Connexion Établie.");
        }

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

    //-----Initiation pour la lecture de la température---------
    temperatureStub = new TemperatureStub;
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22

    //------Initialisation des LED statuts----------
    pinMode(GPIO_PIN_LED_LOCK_ROUGE,OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN,OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_YELLOW,OUTPUT);

    //------Conversion Temperature Minimal en string--------
    sprintf(bufferTemperatureMin,"%2.1f",temperatureMin);
    stringTemperatureMin = String(bufferTemperatureMin).c_str();
 }

void loop() {
    float t = temperatureStub->getTemperature();
    
    sprintf(bufferTemperature,"%2.1f",t);
    temperature = String(bufferTemperature).c_str();

    if (demarrer == 0)
    {
        //Status arrêter au démarrage du système
        digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
        bkColor="red";
    }    
      
    if(demarrer == 1){     //démarre juste si bouton démarrer appuyer                
        if(tempsSechage <= tempsSechageMax){ 
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
            //permet de valider la température min/max sinon pause          
            if((t >= (temperatureMin-(temperatureMin*0.1)))&&(t <= (temperatureMin+(temperatureMin*0.1)))){
                tempsSechage++; //ajout d'une seconde
                sprintf(bufferTemps,"%i",tempsSechage);
                temps = String(bufferTemps).c_str();
                digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW);
                bkColor="green";
            }
            else {
                digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
                bkColor="yellow";
            }
        }  
        //affichage du temps de séchage actuelle               
        Serial.println(tempsSechage); 
        delay(500);   
        //réinitialisation des variable lorsque le temps de séchage est atteint
        if(tempsSechage >= tempsSechageMax){
            demarrer=0;
            tempsSechage=0;
            digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
            bkColor="red";
            //mettre code pour Web pour afficher four prêt à être utiliser
        }              
    }
      
    delay(500);      
  }
