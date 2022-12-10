/**
    Class MyOledViewErrorWifiConnexion : Initialisation des valeur pour le OLed
    @file MyOledViewErrorWifiConnexion.h 
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
            #include "MyOledViewErrorWifiConnexion.h"
            class MyOledViewErrorWifiConnexion: public MyOledView {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewErrorWifiConnexion = new MyOledViewErrorWifiConnexion();
        myOled->displayView(myOledViewInitialisation);
**/

#define MYOLEDVIEWERRORWIFICONNEXION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>

class MyOledViewErrorWifiConnexion: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);

    private:
        std::string nomDuSysteme; 
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);         
};