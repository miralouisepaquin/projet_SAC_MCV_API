/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorkingHeat.cpp
    @author Mira Paquin
    @version 1.1 01/12/2022
*/
#include <Arduino.h>
#include "MyOledViewWorkingHeat.h"

using namespace std;

void MyOledViewWorkingHeat::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWorkingHeat");
    }

void MyOledViewWorkingHeat::display( Adafruit_SSD1306 *adafruit) {
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());

    adafruit->setCursor(70, 20);
    adafruit->print("Heating");

    adafruit->setTextSize(2);
    adafruit->setCursor(50, 30);
    adafruit->print(getTag("temperature").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(40, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());

    adafruit->display();
    }