/**
    Class MyOledViewWorkingOFF : Initialisation des valeur pour le OLed
    @file MyOledViewWorkingOFF.h 
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
            #include "MyOledViewWorkingOFF.h"
            class MyOledViewWorkingOFF: public MyOledViewWorking {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewWorkingOFF = new MyOledViewWorkingOFF();
        myOled->displayView(myOledViewWorkingOFF);
**/

#define MYOLEDVIEWWORKINOFF_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"
#include <string>

class MyOledViewWorkingOFF: public MyOledViewWorking {
    
    public:

    private:  
        void display(Adafruit_SSD1306 *adafruit);        
};