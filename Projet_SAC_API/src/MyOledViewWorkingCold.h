/**
    Class MyOledViewWorkingCold : Initialisation des valeur pour le OLed
    @file MyOledViewWorkingCold.h 
    @author Mira Paquin
    @version 1.1 01/12/2022 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      01/12/2022   MLP         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        //Définition 
            #include "MyOledViewWorkingCold.h"
            class MyOledViewWorkingCold: public MyOledViewWorking {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewWorkingCold = new MyOledViewWorkingCold();
        myOled->displayView(myOledViewWorkingCold);
**/

#define MYOLEDVIEWWORKINGCOLD_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"
#include <string>

class MyOledViewWorkingCold: public MyOledViewWorking {
    
    public:

    private:  
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);         
};