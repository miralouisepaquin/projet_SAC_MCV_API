/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWifiAp.cpp
    @author Mira Paquin
    @version 1.1 01/12/2022
*/
#include <Arduino.h>
#include "MyOledViewWifiAp.h"

using namespace std;

void MyOledViewWifiAp::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
    }
void MyOledViewWifiAp::setSsIdDuSysteme(std::string val){
    ssIdDuSysteme = val;
}
void MyOledViewWifiAp::setPassDuSysteme(std::string val){
    passDuSysteme = val;
    }

void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiAp");
    }

void MyOledViewWifiAp::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWorking");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0,0);
    adafruit->print(nomDuSysteme.c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0,20);
    adafruit->print("ID: ");
    adafruit->print(nomDuSysteme.c_str());

    adafruit->setCursor(0,30);
    adafruit->print("AP Configuration");
    
    adafruit->setCursor(30,40);
    adafruit->print("SSID: ");
    adafruit->print(ssIdDuSysteme.c_str());

    adafruit->setCursor(30,50);
    adafruit->print("Pass: ");
    adafruit->print(passDuSysteme.c_str());

    adafruit->display();
    }