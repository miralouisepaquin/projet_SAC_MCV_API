/**
    Class MyOledViewWifiAp : Initialisation des valeur pour le OLed
    @file MyOledViewWifiAp.h 
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
            #include "MyOledViewWifiAp.h"
            class MyOledViewWifiAp: public MyOledView {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewWifiAp = new MyOledViewWifiAp();
        myOled->displayView(myOledViewInitialisation);
**/

#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>

class MyOledViewWifiAp: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setSsIdDuSysteme(std::string val);
        void setPassDuSysteme(std::string val);

    private:
        std::string nomDuSysteme;
        std::string ssIdDuSysteme;
        std::string passDuSysteme;   
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);         
};