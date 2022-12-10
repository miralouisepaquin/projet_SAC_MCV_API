/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWifiAp.cpp
    @author Mira Paquin
    @version 1.1 01/12/2022
*/
#include <Arduino.h>
#include "MyOledViewErrorWifiConnexion.h"

using namespace std;

void MyOledViewErrorWifiConnexion::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
    }

void MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewErrorWifiConnexion");
    }

void MyOledViewErrorWifiConnexion::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewErrorWifiConnexion");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0,0);
    adafruit->print(nomDuSysteme.c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0,20);
    adafruit->print("Erreur Connexion Wifi");

    adafruit->display();
    }