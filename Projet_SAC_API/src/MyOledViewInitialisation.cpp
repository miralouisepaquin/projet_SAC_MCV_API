/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewInitialisation.cpp
    @author Mira Paquin
    @version 1.1 01/12/2022
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"

using namespace std;

void MyOledViewInitialisation::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
    }
void MyOledViewInitialisation::setIdDuSysteme(std::string val){
    idDuSysteme = val;
}
void MyOledViewInitialisation::setSensibiliteBoutonAction(std::string val){
    SensibiliteBoutonAction = val;
    }
void MyOledViewInitialisation::setSensibiliteBoutonReset(std::string val){
    SensibiliteBoutonReset = val;
    }

void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWorking");
    }

void MyOledViewInitialisation::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWorking");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0,0);
    adafruit->print(nomDuSysteme.c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0,20);
    adafruit->print("Id: ");
    adafruit->print(idDuSysteme.c_str());
    
    adafruit->setCursor(0,30);
    adafruit->print("Bouton Action: ");
    adafruit->print(SensibiliteBoutonAction.c_str());

    adafruit->setCursor(0,40);
    adafruit->print("Bouton Reset: ");
    adafruit->print(SensibiliteBoutonReset.c_str());

    adafruit->display();
    }